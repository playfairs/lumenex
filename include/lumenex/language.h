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
    LUMENEX_LANGUAGE_JAVASCRIPT = 5
} lumenex_language_t;

LUMENEX_API const char *
lumenex_language_name(lumenex_language_t language);
LUMENEX_API lumenex_language_t
lumenex_language_from_name(const char *name);

#endif
