# Lexer

Lexer is the lexical parser for the violence language. Its job is to parse the source code into tokens.

## How it works

Lexer goes through the source code and when encounters a different type of token it will add the previous token value to token list.

## Tokens

All token types are defined in `Token.h` and are:

```c
KEYWORD,
TYPE,
IDENTIFIER,
OPERATOR,
DELIMITER,
LITERAL_STRING,
LITERAL_NUMBER,
FUNCTION_START,
FUNCTION_END,
COMMENT,
WHITESPACE,
EOL,
ERROR
```
