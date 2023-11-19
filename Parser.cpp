//
// Created by plusleft on 11/8/2023.
//

#include "Parser.h"
#include "InlineCHandler.h"

Parser::Parser() {
    this->interpreter = new Interpreter();
}

Variable Parser::parse(vector<Token> tokens) {
    vector<Token> line_tokens;
    Variable variable = Variable("", "", "");

    bool is_function = false;
    bool skipping = false;

    for (Token token : tokens) {

        if (token.get_type() == KEYWORD && token.get_value() == "f->") {
            if (is_function) {
                error_out("cannot have nested functions");
            }
            is_function = true;
        }
        if (token.get_type() == KEYWORD && token.get_value() == "<-f" && is_function) {
            line_tokens.push_back(token);
            string name = line_tokens[1].get_value();
            vector<string> arg_names;
            vector<Token> body;
            // could have many args so return type is last before the first EOL
            string return_type;
            for (int i = 3; i < line_tokens.size(); i++) {
                if (line_tokens[i].get_type() == EOL) {
                    return_type = line_tokens[i - 1].get_value();
                    break;
                } else if (line_tokens[i].get_type() == IDENTIFIER) {
                    arg_names.push_back(line_tokens[i].get_value());
                }
            }

            // get body
            bool found_start = false;
            for (int i = 0; i < tokens.size(); i++) {
                if (line_tokens[i].get_type() == KEYWORD && line_tokens[i].get_value() == "f->") {
                    found_start = true;
                    continue;
                } else if (line_tokens[i].get_type() == KEYWORD && line_tokens[i].get_value() == "<-f") {
                    break;
                } else if (found_start) {
                    if (line_tokens[i].get_type() == INLINE_C) {
                        // if not compiled then compile
                        InlineCHandler inline_c_handler;
                        bool can_load = inline_c_handler.can_load(name, name);
                        if (!can_load) {
                            inline_c_handler.compile(name);
                        }
                        // add function to interpreter
                        CFunction c_function = CFunction(name, name + string("_c"), return_type);
                        this->interpreter->add_c_function(c_function);

                        // insert function call tokens
                        body.push_back(Token("(", 0, 0, FUNCTION_START));
                        body.push_back(Token(name + string("_c"), 0, 0, IDENTIFIER));
                        body.push_back(Token(")", 0, 0, FUNCTION_END));
                        body.push_back(Token("EOL", 0, 0, EOL));
                        continue;
                    }
                    body.push_back(line_tokens[i]);
                }
            }

            // Token::print_tokens(body);

            Function function = Function(name, arg_names, body, return_type);
            this->interpreter->add_function(function);

            is_function = false;
        }

        if (is_function) {
            line_tokens.push_back(token);
        } else if (token.get_type() == EOL && !skipping) {
            variable = parse_line(line_tokens, token.get_line());
            line_tokens.clear();
            if (variable.get_type() == "skip") {
                skipping = true;
            }
        } else if (token.get_type() == EOL && skipping) {
            skipping = false;
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
    // printf("line: %d\n", line);
    // Token::print_tokens(line_tokens);
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

        // not really
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
    } else if (line_tokens[0].get_type() == KEYWORD) {
        if (line_tokens[0].get_value() == "if") {
            // Token::print_tokens(line_tokens);
            // if false then skip the next line
            // if true then execute the next line

            // check if expression is true
            vector<Token> expression_tokens;

            for (int i = 1; i < line_tokens.size(); i++) {
                if (line_tokens[i].get_type() == EOL) {
                    break;
                } else {
                    expression_tokens.push_back(line_tokens[i]);
                }
            }

            if (expression_tokens.size() == 0) {
                error_out("if statement must have an expression");
            }

            Expression expression = Expression(expression_tokens, "mood", this->interpreter);
            string value = expression.evaluate("mood");

            if (value == "bad") {
                return Variable("", "skip", "");
            } else {
                return Variable("", "execute", "");
            }
        }
    } else if (line_tokens[0].get_type() == FUNCTION_START) {
        Expression expression = Expression(line_tokens, "void", this->interpreter);
        expression.evaluate("void");

        return Variable("", "execute", "");
    }

    return Variable("", "", "");
}