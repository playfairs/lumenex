#ifndef LUMENEX_LANGUAGE_H
#define LUMENEX_LANGUAGE_H

#include <stddef.h>

#include "lumenex/export.h"

typedef enum lumenex_language_t
{
    LUMENEX_LANGUAGE_AUTO = 0,
    LUMENEX_LANGUAGE_C = 1,
    LUMENEX_LANGUAGE_CPP = 2,
    LUMENEX_LANGUAGE_RUST = 3,
    LUMENEX_LANGUAGE_PYTHON = 4,
    LUMENEX_LANGUAGE_JAVASCRIPT = 5,
    LUMENEX_LANGUAGE_TYPESCRIPT = 6,
    LUMENEX_LANGUAGE_CS = 7,
    LUMENEX_LANGUAGE_GO = 8,
    LUMENEX_LANGUAGE_ZIG = 9,
    LUMENEX_LANGUAGE_ADA = 10,
    LUMENEX_LANGUAGE_JAVA = 11,
    LUMENEX_LANGUAGE_KOTLIN = 12,
    LUMENEX_LANGUAGE_LUA = 13,
    LUMENEX_LANGUAGE_SWIFT = 14,
    LUMENEX_LANGUAGE_DART = 15,
    LUMENEX_LANGUAGE_GLSL = 16,
    LUMENEX_LANGUAGE_HASKELL = 17
} lumenex_language_t;

LUMENEX_API const char *
lumenex_language_name(lumenex_language_t language);
LUMENEX_API lumenex_language_t
lumenex_language_from_name(const char *name);

#endif
