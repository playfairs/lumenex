#ifndef LUMENEX_LANGUAGES_ZIG_LEXER_H
#define LUMENEX_LANGUAGES_ZIG_LEXER_H

#include "lumenex/lexer.h"

int lumenex_zig_lex(const char *source,
                    size_t length,
                    const lumenex_lexer_options_t *options,
                    lumenex_lex_result_t *result);

#endif
