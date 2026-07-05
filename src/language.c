#include "lumenex/language.h"

const char *
lumenex_language_name(lumenex_language_t language)
{
    switch (language)
    {
    case LUMENEX_LANGUAGE_C:
        return "c";
    case LUMENEX_LANGUAGE_CPP:
        return "cpp";
    case LUMENEX_LANGUAGE_RUST:
        return "rust";
    case LUMENEX_LANGUAGE_PYTHON:
        return "python";
    case LUMENEX_LANGUAGE_JAVASCRIPT:
        return "javascript";
    case LUMENEX_LANGUAGE_AUTO:
    default:
        return "auto";
    }
}

lumenex_language_t
lumenex_language_from_name(const char *name)
{
    if (name == NULL)
    {
        return LUMENEX_LANGUAGE_AUTO;
    }
    if (name[0] == 'c' && name[1] == '\0')
    {
        return LUMENEX_LANGUAGE_C;
    }
    if (name[0] == 'C' && name[1] == '\0')
    {
        return LUMENEX_LANGUAGE_C;
    }
    if (name[0] == 'c' && name[1] == '+' && name[2] == '+'
        && name[3] == '\0')
    {
        return LUMENEX_LANGUAGE_CPP;
    }
    if (name[0] == 'r' && name[1] == 'u' && name[2] == 's'
        && name[3] == 't' && name[4] == '\0')
    {
        return LUMENEX_LANGUAGE_RUST;
    }
    if (name[0] == 'p' && name[1] == 'y' && name[2] == 't'
        && name[3] == 'h' && name[4] == 'o'
        && name[5] == 'n' && name[6] == '\0')
    {
        return LUMENEX_LANGUAGE_PYTHON;
    }
    if (name[0] == 'j' && name[1] == 's' && name[2] == '\0')
    {
        return LUMENEX_LANGUAGE_JAVASCRIPT;
    }
    return LUMENEX_LANGUAGE_AUTO;
}
