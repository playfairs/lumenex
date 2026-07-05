#include "src/languages/java/lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static const char *java_keywords[] = {
    "abstract", "assert",       "boolean",  "break",
    "byte",     "case",         "catch",    "char",
    "class",    "const",        "continue", "default",
    "do",       "double",       "else",     "enum",
    "extends",  "final",        "finally",  "float",
    "for",      "goto",         "if",       "implements",
    "import",   "instanceof",   "int",      "interface",
    "long",     "native",       "new",      "package",
    "private",  "protected",    "public",   "return",
    "short",    "static",       "strictfp", "super",
    "switch",   "synchronized", "this",     "throw",
    "throws",   "transient",    "try",      "void",
    "volatile", "while",        NULL};

static int is_java_keyword(const char *begin,
                           const char *end)
{
    size_t len = (size_t)(end - begin);
    for (const char **k = java_keywords; *k != NULL; ++k)
    {
        if (strlen(*k) == len
            && memcmp(begin, *k, len) == 0)
            return 1;
    }
    return 0;
}

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

int lumenex_java_lex(const char *source,
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

        if (c == '/')
        {
            if (cursor + 1 < end && cursor[1] == '/')
            {
                const char *comment_end = cursor;
                while (comment_end < end
                       && *comment_end != '\n')
                    comment_end += 1;
                if (options != NULL
                    && options->preserve_comments)
                {
                    lumenex_token_t token =
                        lumenex_make_token(
                            LUMENEX_TOKEN_COMMENT,
                            start,
                            comment_end,
                            start_line,
                            start_column);
                    if (!lumenex_append_token(
                            &result->tokens,
                            token))
                    {
                        result->had_error = 1;
                        result->error_message =
                            "out of memory";
                        return 0;
                    }
                }
                cursor = comment_end;
                continue;
            }
            if (cursor + 1 < end && cursor[1] == '*')
            {
                const char *comment_end = cursor + 2;
                while (comment_end + 1 < end
                       && !(comment_end[0] == '*'
                            && comment_end[1] == '/'))
                    comment_end += 1;
                if (comment_end + 1 < end)
                    comment_end += 2;
                if (options != NULL
                    && options->preserve_comments)
                {
                    lumenex_token_t token =
                        lumenex_make_token(
                            LUMENEX_TOKEN_COMMENT,
                            start,
                            comment_end,
                            start_line,
                            start_column);
                    if (!lumenex_append_token(
                            &result->tokens,
                            token))
                    {
                        result->had_error = 1;
                        result->error_message =
                            "out of memory";
                        return 0;
                    }
                }
                cursor = comment_end;
                continue;
            }
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
                if (*literal_end == '\n')
                {
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
            column += (size_t)(literal_end - start);
            continue;
        }

        if (c == '\'')
        {
            const char *char_end = cursor + 1;
            if (char_end < end && *char_end == '\\')
                char_end += 2;
            if (char_end < end && *char_end == '\'')
                char_end += 1;
            lumenex_token_t token =
                lumenex_make_token(LUMENEX_TOKEN_CHARACTER,
                                   start,
                                   char_end,
                                   start_line,
                                   start_column);
            if (!lumenex_append_token(&result->tokens,
                                      token))
            {
                result->had_error = 1;
                result->error_message = "out of memory";
                return 0;
            }
            cursor = char_end;
            column += (size_t)(char_end - start);
            continue;
        }

        if (isalnum(c) || c == '_')
        {

            if (isdigit(c))
            {
                const char *num_end = cursor;
                if (*num_end == '0' && num_end + 1 < end
                    && (num_end[1] == 'x'
                        || num_end[1] == 'X'))
                {
                    num_end += 2;
                    while (num_end < end
                           && (isxdigit(
                                   (unsigned char)*num_end)
                               || *num_end == '_'))
                        num_end += 1;
                }
                else
                {
                    while (
                        num_end < end
                        && (isdigit((unsigned char)*num_end)
                            || *num_end == '_'))
                        num_end += 1;
                    if (num_end < end && *num_end == '.')
                    {
                        num_end += 1;
                        while (
                            num_end < end
                            && (isdigit(
                                    (unsigned char)*num_end)
                                || *num_end == '_'))
                            num_end += 1;
                    }
                    if (num_end < end
                        && (*num_end == 'e'
                            || *num_end == 'E'))
                    {
                        num_end += 1;
                        if (num_end < end
                            && (*num_end == '+'
                                || *num_end == '-'))
                            num_end += 1;
                        while (num_end < end
                               && isdigit(
                                   (unsigned char)*num_end))
                            num_end += 1;
                    }
                    if (num_end < end
                        && (num_end[0] == 'f'
                            || num_end[0] == 'F'
                            || num_end[0] == 'd'
                            || num_end[0] == 'D'
                            || num_end[0] == 'l'
                            || num_end[0] == 'L'))
                        num_end += 1;
                }
                lumenex_token_t token =
                    lumenex_make_token(LUMENEX_TOKEN_NUMBER,
                                       start,
                                       num_end,
                                       start_line,
                                       start_column);
                if (!lumenex_append_token(&result->tokens,
                                          token))
                {
                    result->had_error = 1;
                    result->error_message = "out of memory";
                    return 0;
                }
                column += (size_t)(num_end - start);
                cursor = num_end;
                continue;
            }
            const char *word_end = cursor + 1;
            while (word_end < end
                   && (isalnum((unsigned char)*word_end)
                       || *word_end == '_'))
                word_end += 1;
            lumenex_token_kind_t kind =
                is_java_keyword(start, word_end)
                    ? LUMENEX_TOKEN_KEYWORD
                    : LUMENEX_TOKEN_IDENTIFIER;
            lumenex_token_t token =
                lumenex_make_token(kind,
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

        if (strchr("+-*/%=&|^!<>.?~", c) != NULL)
        {
            const char *op_end = cursor + 1;
            if (op_end < end)
            {
                if ((c == '+'
                     && (*op_end == '+' || *op_end == '='))
                    || (c == '-'
                        && (*op_end == '-'
                            || *op_end == '='))
                    || (c == '&' && *op_end == '&')
                    || (c == '|' && *op_end == '|')
                    || (c == '=' && *op_end == '=')
                    || (c == '!' && *op_end == '=')
                    || (c == '<'
                        && (*op_end == '<'
                            || *op_end == '='))
                    || (c == '>'
                        && (*op_end == '>'
                            || *op_end == '='))
                    || (c == '*' && *op_end == '=')
                    || (c == '/' && *op_end == '=')
                    || (c == '%' && *op_end == '='))
                {
                    op_end += 1;
                }
            }
            lumenex_token_t token =
                lumenex_make_token(LUMENEX_TOKEN_OPERATOR,
                                   start,
                                   op_end,
                                   start_line,
                                   start_column);
            if (!lumenex_append_token(&result->tokens,
                                      token))
            {
                result->had_error = 1;
                result->error_message = "out of memory";
                return 0;
            }
            column += (size_t)(op_end - start);
            cursor = op_end;
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
