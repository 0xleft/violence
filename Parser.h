//
// Created by plusleft on 11/8/2023.
//

#include "Token.h"
#include <vector>
#include "AST.h"
#include "Interpreter.h"
#include <iostream>

using namespace std;

class Parser {
private:
    Interpreter interpreter;
    int index;

public:
    Parser();
    void parse_line(std::vector<Token> line_tokens, int line);
    void error_out(string error);
    void parse(std::vector<Token> tokens);
    ~Parser();
};