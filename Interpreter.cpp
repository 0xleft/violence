//
// Created by plusleft on 11/6/2023.
//

#include "Interpreter.h"
#include <iostream>
#include <string>

using namespace std;

string Expression::evaluate(string return_type) {
    vector<Token> tokens = this->tokens;
    Interpreter *interpreter = this->interpreter;
    string final_value = "";

    // check return type is valid
    if (return_type != "word" && return_type != "lemon" && return_type != "mood" && return_type != "void") {
        error_out("invalid return type");
    }

    if (tokens[0].get_type() == OPERATOR) {
        error_out("expression cannot start with operator");
    }

    final_value = resolve(tokens[0]);

    if (return_type == "word") {
        // string operations

        for (int i = 1; i < tokens.size(); i++) {
            Token token = tokens[i];
            TokenType type = token.get_type();
            string value = token.get_value();

            if (type == OPERATOR) {
                string next_value = resolve(tokens[i + 1]);

                if (value == "+") {
                    final_value += next_value;
                } else if (value == "-") {
                    final_value = final_value.substr(0, final_value.size() - std::stoi(next_value));
                } else if (value == "*") {
                    string new_value = "";
                    for (int i = 0; i < std::stoi(next_value); i++) {
                        new_value += final_value;
                    }
                    final_value = new_value;
                } else if (value == "/") {
                    final_value = final_value.substr(0, final_value.size() / std::stoi(next_value));
                } else if (value == "%") {
                    final_value = final_value.substr(std::stoi(next_value), final_value.size());
                } else if (value == "^") {
                    final_value = final_value.substr(0, std::stoi(next_value));
                }
            }
        }

    } else if (return_type == "lemon") {
        // math operations

        for (int i = 1; i < tokens.size(); i++) {
            Token token = tokens[i];
            TokenType type = token.get_type();
            string value = token.get_value();

            if (type == OPERATOR) {
                string next_value = resolve(tokens[i + 1]);

                if (value == "+") {
                    final_value = std::to_string(std::stof(final_value) + std::stof(next_value));
                } else if (value == "-") {
                    final_value = std::to_string(std::stof(final_value) - std::stof(next_value));
                } else if (value == "*") {
                    final_value = std::to_string(std::stof(final_value) * std::stof(next_value));
                } else if (value == "/") {
                    final_value = std::to_string(std::stof(final_value) / std::stof(next_value));
                } else if (value == "%") {
                    final_value = std::to_string(std::stoi(final_value) % std::stoi(next_value));
                } else if (value == "^") {
                    final_value = std::to_string(std::stoi(final_value) ^ std::stoi(next_value));
                }
            }
        }
    } else if (return_type == "mood") {
        // boolean operations good == true, bad == false
        for (int i = 1; i < tokens.size(); i++) {
            Token token = tokens[i];
            TokenType type = token.get_type();
            string value = token.get_value();

            if (type == OPERATOR) {
                string next_value = resolve(tokens[i + 1]);

                if (value == "==") {
                    if (final_value == next_value) {
                        final_value = "good";
                    } else {
                        final_value = "bad";
                    }
                } else if (value == "!=") {
                    if (final_value != next_value) {
                        final_value = "good";
                    } else {
                        final_value = "bad";
                    }
                } else if (value == ">") {
                    if (std::stoi(final_value) > std::stoi(next_value)) {
                        final_value = "good";
                    } else {
                        final_value = "bad";
                    }
                } else if (value == "<") {
                    if (std::stoi(final_value) < std::stoi(next_value)) {
                        final_value = "good";
                    } else {
                        final_value = "bad";
                    }
                } else if (value == ">=") {
                    if (std::stoi(final_value) >= std::stoi(next_value)) {
                        final_value = "good";
                    } else {
                        final_value = "bad";
                    }
                } else if (value == "<=") {
                    if (std::stoi(final_value) <= std::stoi(next_value)) {
                        final_value = "good";
                    } else {
                        final_value = "bad";
                    }
                }
            }
        }
    } else if (return_type == "void") {
        // void operations
    }

    return final_value;
}

string Expression::resolve(Token token) {
    string value = token.get_value();
    TokenType type = token.get_type();

    if (type == IDENTIFIER) {
        // print token
        // check if variable exists
        if (this->interpreter->get_global_scope()->get_variable(value).get_name() != "") {
            return this->interpreter->get_global_scope()->get_variable(value).get_value();
        } else {
            error_out("variable \"" + value + "\" does not exist");
        }
    } else if (type == LITERAL_STRING || type == LITERAL_NUMBER) {
        return value;
    }

    return "";
}

void Expression::error_out(string error) {
    cout << "Interpreter Error: " << error << endl;
    exit(1);
}