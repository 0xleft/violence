# Parser

The parser is the part of the interpreter that parses the tokens into expressions.

## How it works

The parser works by looking at the first token and then determining what the expressions is.

So for example if the first token is a `word` then it will parse it as a `word` expression.