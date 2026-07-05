#include "src/languages/javascript/lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static int
lumenex_append_token(lumenex_token_list_t *tokens,
                     lumenex_token_t token)
{
    if (tokens->count == tokens->capacity)
    {
        size_t new_capacity = tokens->capacity == 0
                                  ? 16
                                  : tokens->capacity * 2;
        lumenex_token_t *new_items =
            (lumenex_token_t *)realloc(
                tokens->items,
                new_capacity * sizeof(*new_items));
        if (new_items == NULL)
        {
            return 0;
        }
        tokens->items = new_items;
        tokens->capacity = new_capacity;
    }
    tokens->items[tokens->count++] = token;
    return 1;
}

static lumenex_token_t
lumenex_make_token(lumenex_token_kind_t kind,
                   const char *begin,
                   const char *end,
                   size_t line,
                   size_t column)
{
    lumenex_token_t token;
    token.kind = kind;
    token.span.begin = begin;
    token.span.end = end;
    token.line = line;
    token.column = column;
    token.length = (size_t)(end - begin);
    token.flags = 0;
    return token;
}

int lumenex_javascript_lex(
    const char *source,
    size_t length,
    const lumenex_lexer_options_t *options,
    lumenex_lex_result_t *result)
{
    const char *cursor = source;
    const char *end = source + length;
    size_t line = 1;
    size_t column = 1;
    (void)options;

    if (source == NULL || result == NULL)
    {
        return 0;
    }

    while (cursor < end)
    {
        const char *start = cursor;
        size_t start_line = line;
        size_t start_column = column;
        unsigned char c = (unsigned char)*cursor;

        if (isspace(c))
        {
            while (cursor < end
                   && isspace((unsigned char)*cursor))
            {
                if (*cursor == '\n')
                {
                    line += 1;
                    column = 1;
                }
                else
                {
                    column += 1;
                }
                cursor += 1;
            }
            if (options != NULL
                && options->preserve_whitespace)
            {
                lumenex_token_t token = lumenex_make_token(
                    LUMENEX_TOKEN_WHITESPACE,
                    start,
                    cursor,
                    start_line,
                    start_column);
                if (!lumenex_append_token(&result->tokens,
                                          token))
                {
                    result->had_error = 1;
                    result->error_message = "out of memory";
                    return 0;
                }
            }
            continue;
        }

        if (c == '/' && cursor + 1 < end
            && cursor[1] == '/')
        {
            const char *comment_end = cursor;
            while (comment_end < end
                   && *comment_end != '\n')
            {
                comment_end += 1;
            }
            if (options != NULL
                && options->preserve_comments)
            {
                lumenex_token_t token = lumenex_make_token(
                    LUMENEX_TOKEN_COMMENT,
                    start,
                    comment_end,
                    start_line,
                    start_column);
                if (!lumenex_append_token(&result->tokens,
                                          token))
                {
                    result->had_error = 1;
                    result->error_message = "out of memory";
                    return 0;
                }
            }
            cursor = comment_end;
            continue;
        }

        if (c == '"')
        {
            const char *literal_end = cursor + 1;
            while (literal_end < end)
            {
                if (*literal_end == '\\')
                {
                    literal_end += 2;
                    continue;
                }
                if (*literal_end == '"')
                {
                    literal_end += 1;
                    break;
                }
                literal_end += 1;
            }
            lumenex_token_t token =
                lumenex_make_token(LUMENEX_TOKEN_STRING,
                                   start,
                                   literal_end,
                                   start_line,
                                   start_column);
            if (!lumenex_append_token(&result->tokens,
                                      token))
            {
                result->had_error = 1;
                result->error_message = "out of memory";
                return 0;
            }
            cursor = literal_end;
            continue;
        }

        if (isalnum(c) || c == '_')
        {
            const char *word_end = cursor + 1;
            while (word_end < end
                   && (isalnum((unsigned char)*word_end)
                       || *word_end == '_'))
            {
                word_end += 1;
            }
            lumenex_token_t token =
                lumenex_make_token(LUMENEX_TOKEN_IDENTIFIER,
                                   start,
                                   word_end,
                                   start_line,
                                   start_column);
            if (!lumenex_append_token(&result->tokens,
                                      token))
            {
                result->had_error = 1;
                result->error_message = "out of memory";
                return 0;
            }
            cursor = word_end;
            column += (size_t)(word_end - start);
            continue;
        }

        if (c == '(' || c == ')' || c == '{' || c == '}'
            || c == '[' || c == ']' || c == ',' || c == ';'
            || c == ':')
        {
            lumenex_token_t token = lumenex_make_token(
                LUMENEX_TOKEN_PUNCTUATION,
                start,
                cursor + 1,
                start_line,
                start_column);
            if (!lumenex_append_token(&result->tokens,
                                      token))
            {
                result->had_error = 1;
                result->error_message = "out of memory";
                return 0;
            }
            cursor += 1;
            column += 1;
            continue;
        }

        lumenex_token_t token =
            lumenex_make_token(LUMENEX_TOKEN_ERROR,
                               start,
                               cursor + 1,
                               start_line,
                               start_column);
        if (!lumenex_append_token(&result->tokens, token))
        {
            result->had_error = 1;
            result->error_message = "out of memory";
            return 0;
        }
        cursor += 1;
        column += 1;
    }

    lumenex_token_t eof =
        lumenex_make_token(LUMENEX_TOKEN_EOF,
                           end,
                           end,
                           line,
                           column);
    if (!lumenex_append_token(&result->tokens, eof))
    {
        result->had_error = 1;
        result->error_message = "out of memory";
        return 0;
    }
    return 1;
}
