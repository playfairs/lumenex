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
    case LUMENEX_LANGUAGE_TYPESCRIPT:
        return "typescript";
    case LUMENEX_LANGUAGE_CS:
        return "csharp";
    case LUMENEX_LANGUAGE_GO:
        return "go";
    case LUMENEX_LANGUAGE_ZIG:
        return "zig";
    case LUMENEX_LANGUAGE_ADA:
        return "ada";
    case LUMENEX_LANGUAGE_JAVA:
        return "java";
    case LUMENEX_LANGUAGE_KOTLIN:
        return "kotlin";
    case LUMENEX_LANGUAGE_LUA:
        return "lua";
    case LUMENEX_LANGUAGE_SWIFT:
        return "swift";
    case LUMENEX_LANGUAGE_DART:
        return "dart";
    case LUMENEX_LANGUAGE_GLSL:
        return "glsl";
    case LUMENEX_LANGUAGE_HASKELL:
        return "haskell";
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
    if (name[0] == 't' && name[1] == 's' && name[2] == '\0')
    {
        return LUMENEX_LANGUAGE_TYPESCRIPT;
    }
    if (name[0] == 'c' && name[1] == '#' && name[2] == '\0')
    {
        return LUMENEX_LANGUAGE_CS;
    }
    if (name[0] == 'g' && name[1] == 'o' && name[2] == '\0')
    {
        return LUMENEX_LANGUAGE_GO;
    }
    if (name[0] == 'z' && name[1] == 'i' && name[2] == 'g'
        && name[3] == '\0')
    {
        return LUMENEX_LANGUAGE_ZIG;
    }
    if (name[0] == 'a' && name[1] == 'd' && name[2] == 'a'
        && name[3] == '\0')
    {
        return LUMENEX_LANGUAGE_ADA;
    }
    if (name[0] == 'j' && name[1] == 'a' && name[2] == 'v'
        && name[3] == 'a' && name[4] == '\0')
    {
        return LUMENEX_LANGUAGE_JAVA;
    }
    if (name[0] == 'k' && name[1] == 'o' && name[2] == 't'
        && name[3] == 'l' && name[4] == 'i'
        && name[5] == 'n' && name[6] == '\0')
    {
        return LUMENEX_LANGUAGE_KOTLIN;
    }
    if (name[0] == 'l' && name[1] == 'u' && name[2] == 'a'
        && name[3] == '\0')
    {
        return LUMENEX_LANGUAGE_LUA;
    }
    if (name[0] == 's' && name[1] == 'w' && name[2] == 'i'
        && name[3] == 'f' && name[4] == 't'
        && name[5] == '\0')
    {
        return LUMENEX_LANGUAGE_SWIFT;
    }
    if (name[0] == 'd' && name[1] == 'a' && name[2] == 'r'
        && name[3] == 't' && name[4] == '\0')
    {
        return LUMENEX_LANGUAGE_DART;
    }
    if (name[0] == 'g' && name[1] == 'l' && name[2] == 's'
        && name[3] == 'l' && name[4] == '\0')
    {
        return LUMENEX_LANGUAGE_GLSL;
    }
    if (name[0] == 'h' && name[1] == 'a' && name[2] == 's'
        && name[3] == 'k' && name[4] == 'e'
        && name[5] == 'l' && name[6] == 'l'
        && name[7] == '\0')
    {
        return LUMENEX_LANGUAGE_HASKELL;
    }
    return LUMENEX_LANGUAGE_AUTO;
}
