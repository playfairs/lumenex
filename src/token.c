#include "lumenex/token.h"

#include <stdlib.h>

const char *
lumenex_token_kind_name(lumenex_token_kind_t kind)
{
    switch (kind)
    {
    case LUMENEX_TOKEN_KEYWORD:
        return "keyword";
    case LUMENEX_TOKEN_IDENTIFIER:
        return "identifier";
    case LUMENEX_TOKEN_NUMBER:
        return "number";
    case LUMENEX_TOKEN_STRING:
        return "string";
    case LUMENEX_TOKEN_CHARACTER:
        return "character";
    case LUMENEX_TOKEN_COMMENT:
        return "comment";
    case LUMENEX_TOKEN_OPERATOR:
        return "operator";
    case LUMENEX_TOKEN_PUNCTUATION:
        return "punctuation";
    case LUMENEX_TOKEN_WHITESPACE:
        return "whitespace";
    case LUMENEX_TOKEN_PREPROCESSOR:
        return "preprocessor";
    case LUMENEX_TOKEN_ATTRIBUTE:
        return "attribute";
    case LUMENEX_TOKEN_ERROR:
        return "error";
    case LUMENEX_TOKEN_EOF:
    default:
        return "eof";
    }
}

void lumenex_token_list_destroy(lumenex_token_list_t *list)
{
    if (list == NULL)
    {
        return;
    }
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void lumenex_lex_result_destroy(
    lumenex_lex_result_t *result)
{
    if (result == NULL)
    {
        return;
    }
    lumenex_token_list_destroy(&result->tokens);
    result->error_count = 0;
    result->had_error = 0;
    result->error_message = NULL;
}
