#include <iostream>
#include "Lexer.h"

int main() {
    Lexer lexer("example/test.vio");
    lexer.lex();
    return 0;
}
