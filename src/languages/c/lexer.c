#include "src/languages/c/lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static int lumenex_is_keyword(const char *text,
                              size_t length)
{
    static const char *keywords[] = {
        "auto",      "break",    "case",     "char",
        "const",     "continue", "default",  "do",
        "double",    "else",     "enum",     "extern",
        "float",     "for",      "goto",     "if",
        "inline",    "int",      "long",     "register",
        "restrict",  "return",   "short",    "signed",
        "sizeof",    "static",   "struct",   "switch",
        "typedef",   "union",    "unsigned", "void",
        "volatile",  "while",    "_Bool",    "_Complex",
        "_Imaginary"};
    size_t i;
    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]);
         ++i)
    {
        if (length == strlen(keywords[i])
            && strncmp(text, keywords[i], length) == 0)
        {
            return 1;
        }
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

static int lumenex_is_identifier_start(unsigned char c)
{
    return isalpha(c) || c == '_';
}

static int lumenex_is_identifier_continue(unsigned char c)
{
    return isalnum(c) || c == '_';
}

static int lumenex_is_digit(unsigned char c)
{
    return isdigit(c);
}

static int lumenex_is_operator_char(unsigned char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/'
           || c == '%' || c == '=' || c == '!' || c == '<'
           || c == '>' || c == '&' || c == '|' || c == '^'
           || c == '~' || c == '?';
}

static int lumenex_is_punctuation_char(unsigned char c)
{
    return c == '(' || c == ')' || c == '{' || c == '}'
           || c == '[' || c == ']' || c == ',' || c == ':'
           || c == ';' || c == '.';
}

int lumenex_c_lex(const char *source,
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

        if (c == '#'
            && (start == source || *(cursor - 1) == '\n'
                || *(cursor - 1) == '\r'))
        {
            while (cursor < end && *cursor != '\n')
            {
                cursor += 1;
            }
            if (options != NULL
                && options->preserve_comments)
            {
                lumenex_token_t token = lumenex_make_token(
                    LUMENEX_TOKEN_PREPROCESSOR,
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
            if (cursor < end && *cursor == '\n')
            {
                cursor += 1;
                line += 1;
                column = 1;
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
            if (cursor < end && *cursor == '\n')
            {
                cursor += 1;
                line += 1;
                column = 1;
            }
            continue;
        }

        if (c == '/' && cursor + 1 < end
            && cursor[1] == '*')
        {
            const char *comment_end = cursor + 2;
            while (comment_end + 1 < end
                   && !(comment_end[0] == '*'
                        && comment_end[1] == '/'))
            {
                if (*comment_end == '\n')
                {
                    line += 1;
                    column = 1;
                }
                else
                {
                    column += 1;
                }
                comment_end += 1;
            }
            if (comment_end + 1 < end)
            {
                comment_end += 2;
            }
            else
            {
                comment_end = end;
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

        if (c == '\'')
        {
            const char *literal_end = cursor + 1;
            while (literal_end < end)
            {
                if (*literal_end == '\\')
                {
                    literal_end += 2;
                    continue;
                }
                if (*literal_end == '\'')
                {
                    literal_end += 1;
                    break;
                }
                literal_end += 1;
            }
            lumenex_token_t token =
                lumenex_make_token(LUMENEX_TOKEN_CHARACTER,
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

        if (lumenex_is_identifier_start(c))
        {
            const char *identifier_end = cursor + 1;
            while (identifier_end < end
                   && lumenex_is_identifier_continue(
                       (unsigned char)*identifier_end))
            {
                identifier_end += 1;
            }
            if (lumenex_is_keyword(
                    start,
                    (size_t)(identifier_end - start)))
            {
                lumenex_token_t token = lumenex_make_token(
                    LUMENEX_TOKEN_KEYWORD,
                    start,
                    identifier_end,
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
            else
            {
                lumenex_token_t token = lumenex_make_token(
                    LUMENEX_TOKEN_IDENTIFIER,
                    start,
                    identifier_end,
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
            cursor = identifier_end;
            column += (size_t)(identifier_end - start);
            continue;
        }

        if (lumenex_is_digit(c)
            || (c == '.' && cursor + 1 < end
                && lumenex_is_digit(
                    (unsigned char)cursor[1])))
        {
            const char *number_end = cursor + 1;
            int has_dot = 0;
            int has_exponent = 0;
            int seen_digit = 0;
            while (number_end < end)
            {
                unsigned char ch =
                    (unsigned char)*number_end;
                if (isdigit(ch))
                {
                    seen_digit = 1;
                    number_end += 1;
                    continue;
                }
                if (ch == '.' && !has_dot && !has_exponent)
                {
                    has_dot = 1;
                    number_end += 1;
                    continue;
                }
                if ((ch == 'e' || ch == 'E') && seen_digit
                    && !has_exponent)
                {
                    has_exponent = 1;
                    number_end += 1;
                    if (number_end < end
                        && (*number_end == '+'
                            || *number_end == '-'))
                    {
                        number_end += 1;
                    }
                    continue;
                }
                break;
            }
            lumenex_token_t token =
                lumenex_make_token(LUMENEX_TOKEN_NUMBER,
                                   start,
                                   number_end,
                                   start_line,
                                   start_column);
            if (!lumenex_append_token(&result->tokens,
                                      token))
            {
                result->had_error = 1;
                result->error_message = "out of memory";
                return 0;
            }
            cursor = number_end;
            column += (size_t)(number_end - start);
            continue;
        }

        if (lumenex_is_operator_char(c))
        {
            const char *operator_end = cursor + 1;
            if ((c == '<' || c == '>' || c == '!'
                 || c == '=' || c == '&' || c == '|')
                && operator_end < end
                && *operator_end == '=')
            {
                operator_end += 1;
            }
            else if ((c == '-' || c == '+' || c == '<'
                      || c == '>' || c == '&' || c == '|')
                     && operator_end < end
                     && *operator_end == c)
            {
                operator_end += 1;
            }
            else if (c == '.' && operator_end < end
                     && *operator_end == '.'
                     && operator_end + 1 < end
                     && operator_end[1] == '.')
            {
                operator_end += 2;
            }
            lumenex_token_t token =
                lumenex_make_token(LUMENEX_TOKEN_OPERATOR,
                                   start,
                                   operator_end,
                                   start_line,
                                   start_column);
            if (!lumenex_append_token(&result->tokens,
                                      token))
            {
                result->had_error = 1;
                result->error_message = "out of memory";
                return 0;
            }
            cursor = operator_end;
            column += (size_t)(operator_end - start);
            continue;
        }

        if (lumenex_is_punctuation_char(c))
        {
            const char *punctuation_end = cursor + 1;
            lumenex_token_t token = lumenex_make_token(
                LUMENEX_TOKEN_PUNCTUATION,
                start,
                punctuation_end,
                start_line,
                start_column);
            if (!lumenex_append_token(&result->tokens,
                                      token))
            {
                result->had_error = 1;
                result->error_message = "out of memory";
                return 0;
            }
            cursor = punctuation_end;
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

    {
        lumenex_token_t token =
            lumenex_make_token(LUMENEX_TOKEN_EOF,
                               end,
                               end,
                               line,
                               column);
        if (!lumenex_append_token(&result->tokens, token))
        {
            result->had_error = 1;
            result->error_message = "out of memory";
            return 0;
        }
    }

    return 1;
}
