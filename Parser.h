//
// Created by plusleft on 11/8/2023.
//

#include "Token.h"
#include <vector>

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    int index;

public:
    Parser(vector<Token> tokens);
    void parse_line(vector<Token> line_tokens, int line);
    void error_out(string error);
    void parse();
    ~Parser();
};

class AST {
private:
    string value;
    vector<AST> children;
};

class BinOp : public AST {
private:
    AST left;
    Token op;
    AST right;
};

class Num : public AST {
private:
    Token token;
};

