//
// Created by plusleft on 11/8/2023.
//

#include "Parser.h"
#include <iostream>

Parser::Parser(vector<Token> tokens) {
    this->tokens = tokens;
    index = 0;
}

void Parser::parse() {
    vector<Token> line_tokens;

    for (Token token : tokens) {
        if (token.get_type() == EOL) {
            parse_line(line_tokens, token.get_line());
            line_tokens.clear();
        } else {
            line_tokens.push_back(token);
        }
    }

}

void Parser::parse_line(vector<Token> line_tokens, int line) {

}

void Parser::error_out(string error) {
    cout << "Parser Error: " << error << endl;
    exit(1);
}

Parser::~Parser() = default;