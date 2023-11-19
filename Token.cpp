//
// Created by plusleft on 11/6/2023.
//

#include "Token.h"
#include "iostream"

Token::Token(string value, int line, int column, TokenType type) {
    this->value = value;
    this->line = line;
    this->column = column;
    this->type = type;
}

string Token::get_value() {
    return value;
}

int Token::get_line() {
    return line;
}

int Token::get_column() {
    return column;
}

TokenType Token::get_type() {
    return type;
}

void Token::set_line(int line) {
    this->line = line;
}

void Token::set_column(int column) {
    this->column = column;
}

string Token::to_string() {
    return "Token(" + value + ", " + std::to_string(line) + ", " + std::to_string(column) + ", " + get_type_string() + ")";
}

string Token::get_type_string() {
    switch (type) {
        case KEYWORD:
            return "KEYWORD";
        case IDENTIFIER:
            return "IDENTIFIER";
        case OPERATOR:
            return "OPERATOR";
        case DELIMITER:
            return "DELIMITER";
        case LITERAL_NUMBER:
            return "LITERAL_NUMBER";
        case LITERAL_STRING:
            return "LITERAL_STRING";
        case FUNCTION_START:
            return "FUNCTION_START";
        case FUNCTION_END:
            return "FUNCTION_END";
        case COMMENT:
            return "COMMENT";
        case INDEXER_START:
            return "INDEXER_START";
        case INDEXER_END:
            return "INDEXER_END";
        case WHITESPACE:
            return "WHITESPACE";
        case ERROR:
            return "ERROR";
        case TYPE:
            return "TYPE";
        case EOL:
            return "EOL";
        default:
            return "UNKNOWN";
    }
}

Token::~Token() = default;

void Token::print_tokens(vector<Token> tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        cout << tokens[i].to_string() << endl;
    }
}