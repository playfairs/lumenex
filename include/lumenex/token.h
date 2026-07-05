#ifndef LUMENEX_TOKEN_H
#define LUMENEX_TOKEN_H

#include <stddef.h>
#include <stdint.h>

#include "lumenex/export.h"
#include "lumenex/language.h"

typedef enum lumenex_token_kind_t
{
    LUMENEX_TOKEN_KEYWORD = 0,
    LUMENEX_TOKEN_IDENTIFIER = 1,
    LUMENEX_TOKEN_NUMBER = 2,
    LUMENEX_TOKEN_STRING = 3,
    LUMENEX_TOKEN_CHARACTER = 4,
    LUMENEX_TOKEN_COMMENT = 5,
    LUMENEX_TOKEN_OPERATOR = 6,
    LUMENEX_TOKEN_PUNCTUATION = 7,
    LUMENEX_TOKEN_WHITESPACE = 8,
    LUMENEX_TOKEN_PREPROCESSOR = 9,
    LUMENEX_TOKEN_ATTRIBUTE = 10,
    LUMENEX_TOKEN_ERROR = 11,
    LUMENEX_TOKEN_EOF = 12
} lumenex_token_kind_t;

typedef struct lumenex_source_range_t
{
    const char *begin;
    const char *end;
} lumenex_source_range_t;

typedef struct lumenex_token_t
{
    lumenex_token_kind_t kind;
    lumenex_source_range_t span;
    size_t line;
    size_t column;
    size_t length;
    uint32_t flags;
} lumenex_token_t;

typedef struct lumenex_token_list_t
{
    lumenex_token_t *items;
    size_t count;
    size_t capacity;
} lumenex_token_list_t;

typedef struct lumenex_lex_result_t
{
    lumenex_token_list_t tokens;
    size_t error_count;
    int had_error;
    const char *error_message;
} lumenex_lex_result_t;

typedef struct lumenex_lexer_options_t
{
    lumenex_language_t language;
    int preserve_whitespace;
    int preserve_comments;
} lumenex_lexer_options_t;

LUMENEX_API const char *
lumenex_token_kind_name(lumenex_token_kind_t kind);
LUMENEX_API void
lumenex_token_list_destroy(lumenex_token_list_t *list);
LUMENEX_API void
lumenex_lex_result_destroy(lumenex_lex_result_t *result);

#endif
