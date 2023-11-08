//
// Created by plusleft on 11/6/2023.
//

#include "Token.h"

using namespace std;

class Lexer {
private:
    char current_char;
    int line;
    int column;
    string content;

public:
    Lexer(string content);
    vector<Token> lex();
    vector<Token> lex_line(string line_content, int line);
    void error(string trace, string message, int line, int column);
    ~Lexer();
};