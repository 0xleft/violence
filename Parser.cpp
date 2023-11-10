//
// Created by plusleft on 11/8/2023.
//

#include "Parser.h"

Parser::Parser() {
    this->interpreter = Interpreter();
}

void Parser::parse(vector<Token> tokens) {
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

void Parser::error_out(string error) {
    cout << "Parser Error: " << error << endl;
    exit(1);
}

Parser::~Parser() = default;

void Parser::parse_line(vector<Token> line_tokens, int line) {
    // if token is type keyword
    printf("line: %d\n", line);
    Token::print_tokens(line_tokens);

    // safety
    if (line_tokens.size() == 0) return;

    if (line_tokens[0].get_type() == TYPE) {
        // that means its a declaration of variable
    }

}