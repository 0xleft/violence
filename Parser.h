//
// Created by plusleft on 11/8/2023.
//

#include "Token.h"
#include <vector>
#include "Interpreter.h"
#include <iostream>

using namespace std;

class Parser {
private:
    Interpreter *interpreter;

public:
    Parser();
    Variable parse_line(std::vector<Token> line_tokens, int line);
    void error_out(string error);
    Variable parse(std::vector<Token> tokens);

    Interpreter *get_interpreter() {
        return this->interpreter;
    }

    ~Parser();
};