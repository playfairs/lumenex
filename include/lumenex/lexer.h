#ifndef LUMENEX_LEXER_H
#define LUMENEX_LEXER_H

#include <stddef.h>

#include "lumenex/export.h"
#include "lumenex/language.h"
#include "lumenex/token.h"

LUMENEX_API lumenex_lex_result_t
lumenex_lex_text(const char *source,
                 size_t length,
                 lumenex_language_t language);
LUMENEX_API lumenex_lex_result_t
lumenex_lex_text_with_options(
    const char *source,
    size_t length,
    const lumenex_lexer_options_t *options);
LUMENEX_API lumenex_lex_result_t
lumenex_lex_file(const char *path,
                 lumenex_language_t language);
LUMENEX_API lumenex_lex_result_t
lumenex_lex_file_with_options(
    const char *path,
    const lumenex_lexer_options_t *options);

#endif
