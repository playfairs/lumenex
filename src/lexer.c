#include "lumenex/lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lumenex/token.h"
#include "src/languages/c/lexer.h"

static lumenex_lexer_options_t
lumenex_default_options(lumenex_language_t language)
{
    lumenex_lexer_options_t options;
    options.language = language;
    options.preserve_whitespace = 0;
    options.preserve_comments = 1;
    return options;
}

static int
lumenex_lex_impl(const char *source,
                 size_t length,
                 const lumenex_lexer_options_t *options,
                 lumenex_lex_result_t *result)
{
    if (source == NULL || result == NULL || options == NULL)
    {
        return 0;
    }

    switch (options->language)
    {
    case LUMENEX_LANGUAGE_C:
        return lumenex_c_lex(source,
                             length,
                             options,
                             result);
    case LUMENEX_LANGUAGE_CPP:
    case LUMENEX_LANGUAGE_RUST:
    case LUMENEX_LANGUAGE_PYTHON:
    case LUMENEX_LANGUAGE_JAVASCRIPT:
    default:
        result->had_error = 1;
        result->error_message = "unsupported language";
        return 0;
    }
}

lumenex_lex_result_t
lumenex_lex_text(const char *source,
                 size_t length,
                 lumenex_language_t language)
{
    lumenex_lexer_options_t options =
        lumenex_default_options(language);
    return lumenex_lex_text_with_options(source,
                                         length,
                                         &options);
}

lumenex_lex_result_t lumenex_lex_text_with_options(
    const char *source,
    size_t length,
    const lumenex_lexer_options_t *options)
{
    lumenex_lex_result_t result;
    memset(&result, 0, sizeof(result));
    if (options == NULL)
    {
        lumenex_lexer_options_t defaults =
            lumenex_default_options(LUMENEX_LANGUAGE_AUTO);
        return lumenex_lex_text_with_options(source,
                                             length,
                                             &defaults);
    }
    if (!lumenex_lex_impl(source, length, options, &result))
    {
        return result;
    }
    return result;
}

lumenex_lex_result_t
lumenex_lex_file(const char *path,
                 lumenex_language_t language)
{
    lumenex_lexer_options_t options =
        lumenex_default_options(language);
    return lumenex_lex_file_with_options(path, &options);
}

lumenex_lex_result_t lumenex_lex_file_with_options(
    const char *path,
    const lumenex_lexer_options_t *options)
{
    FILE *file = NULL;
    char *buffer = NULL;
    long size = 0;
    lumenex_lex_result_t result;
    memset(&result, 0, sizeof(result));

    if (path == NULL)
    {
        result.had_error = 1;
        result.error_message = "path is null";
        return result;
    }

    file = fopen(path, "rb");
    if (file == NULL)
    {
        result.had_error = 1;
        result.error_message = "failed to open file";
        return result;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        result.had_error = 1;
        result.error_message = "failed to seek file";
        return result;
    }
    size = ftell(file);
    if (size < 0)
    {
        fclose(file);
        result.had_error = 1;
        result.error_message = "failed to read file size";
        return result;
    }
    rewind(file);

    buffer = (char *)malloc((size_t)size + 1);
    if (buffer == NULL)
    {
        fclose(file);
        result.had_error = 1;
        result.error_message = "out of memory";
        return result;
    }

    if (fread(buffer, 1, (size_t)size, file)
        != (size_t)size)
    {
        free(buffer);
        fclose(file);
        result.had_error = 1;
        result.error_message = "failed to read file";
        return result;
    }
    buffer[size] = '\0';
    fclose(file);

    result = lumenex_lex_text_with_options(buffer,
                                           (size_t)size,
                                           options);
    free(buffer);
    return result;
}
