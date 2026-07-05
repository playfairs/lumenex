#ifndef LUMENEX_LANGUAGES_TYPESCRIPT_LEXER_H
#define LUMENEX_LANGUAGES_TYPESCRIPT_LEXER_H

#include "lumenex/lexer.h"

int lumenex_typescript_lex(
    const char *source,
    size_t length,
    const lumenex_lexer_options_t *options,
    lumenex_lex_result_t *result);

#endif
