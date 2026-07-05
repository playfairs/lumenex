# LumenEX documentation

**LumenEX** is a standalone lexer library and development project maintained by **[AsterIDE](https://asteride.dev)**. The public API is centered around the lexer entry points in [include/lumenex/lexer.h](../include/lumenex/lexer.h) and the token types in [include/lumenex/token.h](../include/lumenex/token.h).

The library is intentionally limited to lexing only: it accepts source text and a selected language and returns a token stream backed by the original buffer. It does not perform syntax highlighting, rendering, or editor integration, and it is not tied to any specific interface or windowing system.
