#include <stdio.h>
#include <string.h>

#include "lumenex/lexer.h"
#include "lumenex/token.h"

static int check_language(lumenex_language_t language,
                          const char *source)
{
    lumenex_lexer_options_t options;
    options.language = language;
    options.preserve_whitespace = 1;
    options.preserve_comments = 1;
    lumenex_lex_result_t result =
        lumenex_lex_text_with_options(source,
                                      strlen(source),
                                      &options);
    if (result.had_error)
    {
        fprintf(stderr,
                "language %d failed: %s\n",
                (int)language,
                result.error_message != NULL
                    ? result.error_message
                    : "unknown");
        lumenex_lex_result_destroy(&result);
        return 0;
    }
    lumenex_lex_result_destroy(&result);
    return 1;
}

int main(void)
{
    if (!check_language(LUMENEX_LANGUAGE_C,
                        "int main(void) { return 0; }"))
    {
        return 1;
    }
    if (!check_language(LUMENEX_LANGUAGE_CPP,
                        "int main() { return 0; }"))
    {
        return 1;
    }
    if (!check_language(LUMENEX_LANGUAGE_RUST,
                        "fn main() { println!(\"hi\"); }"))
    {
        return 1;
    }
    if (!check_language(LUMENEX_LANGUAGE_PYTHON,
                        "def main():\n    return 0"))
    {
        return 1;
    }
    if (!check_language(LUMENEX_LANGUAGE_JAVASCRIPT,
                        "function main() { return 0; }"))
    {
        return 1;
    }
    return 0;
}
