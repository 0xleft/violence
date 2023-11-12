//
// Created by plusleft on 11/6/2023.
//

#include "Interpreter.h"
#include <iostream>
#include <string>
#include "Parser.h"
#include <algorithm>

using namespace std;

void error_out(string error) {
    cout << "Interpreter Error: " << error << endl;
    exit(1);
}

Interpreter::Interpreter() {
    this->global_scope = new Scope();
    this->functions = vector<Function>();

    // add built in functions
    // ask, say, sheesh get read write

    this->functions.push_back(Function("ask", 1, vector<Token>()));
    this->functions.push_back(Function("say", 1, vector<Token>()));
    this->functions.push_back(Function("sheesh", 1, vector<Token>()));
    this->functions.push_back(Function("get", 1, vector<Token>()));
    this->functions.push_back(Function("read", 1, vector<Token>()));
    this->functions.push_back(Function("write", 2, vector<Token>()));
}

Variable Function::evaluate(vector<Variable> args) {
    if (args.size() != this->get_arg_count()) {
        error_out("invalid number of arguments");
    }

    vector<string> special_functions = {"ask", "say", "sheesh", "get", "read", "write"};

    // if special function
    if (std::find(special_functions.begin(), special_functions.end(), this->name) != special_functions.end()) {
        if (this->name == "ask") {
            // ask
            if (args[0].get_type() != "word") {
                error_out("ask only accepts word type");
            }
            cout << args[0].get_value() << endl;
            string input;
            getline(cin, input);
            return Variable("input", "word", input);
        } else if (this->name == "say") {
            // say
            if (args[0].get_type() != "word") {
                error_out("say only accepts word type");
            }
            cout << args[0].get_value() << endl;
            return Variable("output", "void", "");
        } else if (this->name == "sheesh") {
            // sheesh
            // todo
            if (args[0].get_type() != "word") {
                error_out("sheesh only accepts word type");
            }
            cout << "sheesh" << endl;
            return Variable("output", "void", "");
        } else if (this->name == "get") {
            // get
            // todo
            return Variable("input", "word", args[0].get_value());
        } else if (this->name == "read") {
            // one arg is filename
            if (args[0].get_type() != "word") {
                error_out("read only accepts word type");
            }
            // read
            // todo
            return Variable("input", "word", args[0].get_value());
        } else if (this->name == "write") {
            // two args filename and content
            if (args[0].get_type() != "word" || args[1].get_type() != "word") {
                error_out("write only accepts word type");
            }
            // write
            // todo
            return Variable("output", "void", "");
        }
    }

    // normal function
    Parser parser = Parser();
    for (Variable variable : args) {
        parser.get_interpreter()->get_global_scope()->set_variable(variable);
    }

    Variable output = parser.parse(this->body);
    return output;
}

string Expression::evaluate(string return_type) {
    vector<Token> tokens = this->tokens;

    Token::print_tokens(tokens);

    // check return type is valid
    if (return_type != "word" && return_type != "lemon" && return_type != "mood" && return_type != "void") {
        error_out("invalid return type");
    }

    if (tokens[0].get_type() == OPERATOR) {
        error_out("expression cannot start with operator");
    }

    string final_value = resolve(tokens[0]);

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
        // none just execute
    }

    return final_value;
}

string Expression::resolve_function(vector<Token> tokens) {
    // function is ( name -> arg1 -> arg2 )
    // tokens:
    //Token((, 0, 10, FUNCTION_START)
    //Token(ask, 0, 15, IDENTIFIER)
    //Token(->, 0, 18, OPERATOR)
    //Token(who are you? , 0, 33, LITERAL_STRING)
    //Token(), 0, 35, FUNCTION_END)

    // get function name
    string function_name = tokens[1].get_value();

    // get function
    Function function = this->interpreter->get_function(function_name);
    vector<string> arg_names = function.get_arg_names();

    // get args
    vector<Variable> args = vector<Variable>();
    vector<Token> arg_tokens = vector<Token>();

    // args


    // resolve function
    Variable output = function.evaluate(args);
    return output.get_value();
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