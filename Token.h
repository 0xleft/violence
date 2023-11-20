#pragma once

//
// Created by plusleft on 11/6/2023.
//

#include <string>
#include <vector>

using namespace std;

enum TokenType {
    KEYWORD, //
    TYPE, // int float string bool
    IDENTIFIER, // variable name
    OPERATOR, // + - * / % = == != < > <= >=
    DELIMITER, // ( ) { } [ ] , ; :
    LITERAL_STRING, // 123.456 "abc"
    LITERAL_NUMBER,
    FUNCTION_START,
    FUNCTION_END,
    INDEXER_START,
    INDEXER_END,
    INLINE_C,
    COMMENT,
    WHITESPACE,
    EOL,
    ERROR
};

class Token {
private:
    string value;
    int line;
    int column;
    TokenType type;

public:
    static void print_tokens(vector<Token> tokens);
    Token(string value, int line, int column, TokenType type);
    string get_value();
    int get_line();
    int get_column();
    TokenType get_type();
    string get_type_string();
    void set_line(int line);
    void set_column(int column);
    string to_string();
    ~Token();
};