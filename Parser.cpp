//
// Created by plusleft on 11/8/2023.
//

#include "Parser.h"

Parser::Parser() {
    this->interpreter = new Interpreter();
}

Variable Parser::parse(vector<Token> tokens) {
    vector<Token> line_tokens;
    Variable variable = Variable("", "", "");

    bool is_function = false;

    for (Token token : tokens) {
        if (token.get_type() == EOL) {
            variable = parse_line(line_tokens, token.get_line());
            line_tokens.clear();
        } else {
            line_tokens.push_back(token);
        }
    }

    return variable;
}

void Parser::error_out(string error) {
    cout << "Parser Error: " << error << endl;
    exit(1);
}

Parser::~Parser() {
    delete this->interpreter;
}

Variable Parser::parse_line(vector<Token> line_tokens, int line) {
    // if token is type keyword
    printf("line: %d\n", line);
    // safety
    if (line_tokens.size() == 0) return Variable("", "", "");

    // type name : variable + func + literal
    if (line_tokens[0].get_type() == TYPE) {
        // that means its a variable declaration
        if (line_tokens.size() < 4) {
            error_out("variable declaration must have a name and value and type");
        }

        string name = line_tokens[1].get_value();
        string type = line_tokens[0].get_value();

        // check if variable already exists
        // Variable variable = this->interpreter->get_global_scope()->get_variable(name);
        // if (variable.get_name() != "") {
        //     error_out("variable already exists");
        // }

        // check if type is valid
        if (type != "word" && type != "lemon" && type != "mood" && type != "void") {
            error_out("invalid type");
        }

        // parse value
        string value = "";
        vector<Token> expression_tokens;

        for (int i = 3; i < line_tokens.size(); i++) {
            if (line_tokens[i].get_type() == EOL) {
                break;
            } else {
                expression_tokens.push_back(line_tokens[i]);
            }
        }

        if (expression_tokens.size() == 0) {
            error_out("variable declaration must have a value");
        }

        // parse expression
        Expression expression = Expression(expression_tokens, type, this->interpreter);
        value = expression.evaluate(type);

        // add variable to scope
        Variable new_variable = Variable(name, type, value);
        this->interpreter->get_global_scope()->set_variable(new_variable);

        return new_variable;
    }

    if (line_tokens[0].get_type() == IDENTIFIER) {
        // that means its a function call or variable assignment
    }

    if (line_tokens[0].get_type() == KEYWORD) {
        // that means its a keyword
    }
}