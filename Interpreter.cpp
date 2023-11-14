//
// Created by plusleft on 11/6/2023.
//

#include "Interpreter.h"
#include <iostream>
#include <string>
#include "Parser.h"
#include <algorithm>
#include "Reader.h"

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

    this->functions.push_back(Function("ask", 1, vector<Token>(), "word"));
    this->functions.push_back(Function("say", 1, vector<Token>(), "void"));
    this->functions.push_back(Function("sheesh", 1, vector<Token>(), "word"));
    this->functions.push_back(Function("get", 1, vector<Token>(), "void"));
    this->functions.push_back(Function("read", 1, vector<Token>(), "word"));
    this->functions.push_back(Function("write", 2, vector<Token>(), "void"));
    this->functions.push_back(Function("exit", 0, vector<Token>(), "void"));
}

Variable Function::evaluate(vector<Variable> args, vector<Function> functions, vector<FunctionSymlink> symlinks) {
    if (args.size() != this->get_arg_count()) {
        error_out("invalid number of arguments");
    }

    vector<string> special_functions = {"ask", "say", "sheesh", "get", "read", "write", "exit"};

    // if special function
    if (std::find(special_functions.begin(), special_functions.end(), this->name) != special_functions.end()) {
        if (this->name == "ask") {
            // ask
            if (args[0].get_type() != "word") {
                error_out("ask only accepts word type");
            }
            cout << args[0].get_value();
            string input;
            getline(cin, input);
            return Variable("input", "word", input);
        } else if (this->name == "say") {
            // say
            if (args[0].get_type() != "word") {
                error_out("say only accepts word type");
            }
            cout << args[0].get_value() + "\n";
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
            // import another violence file
            // basicaly just parse the file and add the functions to the interpreter
            //todo
            return Variable("input", "word", args[0].get_value());

        } else if (this->name == "read") {
            // one arg is filename
            if (args[0].get_type() != "word") {
                error_out("read only accepts word type");
            }
            Reader reader = Reader(args[0].get_value());
            string content = reader.get_content();
            reader.close();

            return Variable("output", "word", content);
        } else if (this->name == "write") {
            // two args filename and content
            if (args[0].get_type() != "word" || args[1].get_type() != "word") {
                error_out("write only accepts word type");
            }
            // write
            // todo
            return Variable("output", "void", "");
        } else if (this->name == "exit") {
            // exit
            // todo
            exit(0);
        }
    }

    // normal function
    Parser parser = Parser();
    for (Variable variable : args) {
        parser.get_interpreter()->get_global_scope()->set_variable(variable);
    }

    // functions
    for (Function function : functions) {
        parser.get_interpreter()->add_function(function);
    }

    // symlinks
    for (FunctionSymlink symlink : symlinks) {
        parser.get_interpreter()->set_function_symlink(symlink);
    }

    Variable output = parser.parse(this->body);
    return output;
}

int count_function_calls(vector<Token> tokens) {
    int count = 0;
    for (Token token : tokens) {
        if (token.get_type() == FUNCTION_START) {
            count++;
        }
    }
    return count;
}

string Expression::evaluate(string return_type) {
    vector<Token> tokens = this->tokens;

    // check return type is valid
    if (return_type != "word" && return_type != "lemon" && return_type != "mood" && return_type != "void") {
        error_out("invalid return type");
    }

    if (tokens[0].get_type() == OPERATOR) {
        error_out("expression cannot start with operator");
    }

    // resolve function calls and replace with return value token
    // function call ( name -> arg1 -> arg2 )
    // but could also be ( name -> ( name -> arg1 -> arg2 ) -> arg2 )
    // so we need to resolve the inner function first
    while (count_function_calls(tokens) != 0) {
        // walk through tokens when you find ( look for ) if you find ( again then start over till we find function with ( and ) which means it is inner most function
        // then resolve that function and replace it with the return value
        // then start over

        // find first function call
        int start_index = -1;
        int end_index = -1;

        for (int i = 0; i < tokens.size(); i++) {
            Token token = tokens[i];
            if (token.get_type() == FUNCTION_START) {
                start_index = i + 1;
            } else if (token.get_type() == FUNCTION_END) {
                end_index = i - 1;
                break;
            }
        }

        if (start_index == -1 || end_index == -1) {
            error_out("invalid function call");
        }

        // print tokens in that range
        vector<Token> function_tokens = vector<Token>();
        for (int i = start_index; i <= end_index; i++) {
            function_tokens.push_back(tokens[i]);
        }

        // resolve function
        string function_value = resolve_function(function_tokens);

        // replace function call with return value
        vector<Token> new_tokens = vector<Token>();

        for (int i = 0; i < tokens.size(); i++) {
            Token token = tokens[i];
            if (i == start_index - 1) {
                // replace function call with return value
                new_tokens.push_back(Token(function_value, 0, 0, LITERAL_STRING));
                i = end_index + 1;
            } else {
                new_tokens.push_back(token);
            }
        }

        tokens = new_tokens;
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

                // check if parsable to int or float
                try {
                    std::stoi(final_value);
                } catch (const std::invalid_argument& ia) {
                    error_out("invalid number");
                }

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
    }

    return final_value;
}

string Expression::resolve_function(vector<Token> tokens) {
    // function is name -> arg1 -> arg2
    // tokens:
    //Token(ask, 0, 15, IDENTIFIER)
    //Token(->, 0, 18, OPERATOR)
    //Token(who are you? , 0, 33, LITERAL_STRING)

    // get function name
    string function_name = tokens[0].get_value();

    // get function
    Function function = this->interpreter->get_function(function_name);
    if (function.get_name() == "") {
        if (this->interpreter->get_function_symlink(function_name).get_name() != "") {
            function_name = this->interpreter->get_function_symlink(function_name).get_original_name();
            function = this->interpreter->get_function(function_name);
        }

        error_out("function \"" + function_name + "\" does not exist");
    }

    vector<string> arg_names = function.get_arg_names();

    // get args
    vector<Variable> args = vector<Variable>();
    vector<Token> arg_tokens = vector<Token>();

    vector<FunctionSymlink> symlinks = vector<FunctionSymlink>();

    // args
    int arg_index = 0;
    for (int i = 0; i < tokens.size(); i++) {
        Token token = tokens[i];
        if (token.get_type() == OPERATOR && token.get_value() == "->") {
            if (this->interpreter->get_function(tokens[i + 1].get_value()).get_name() != "") {
                // add empty arg
                args.push_back(Variable(arg_names[arg_index], "skip", ""));
                // add symlink
                FunctionSymlink symlink = FunctionSymlink(arg_names[arg_index], tokens[i + 1].get_value());
                symlinks.push_back(symlink);

                arg_index++;
                continue;
            }

            // resolve arg
            string arg_value = resolve(tokens[i + 1]);
            Variable arg = Variable("", "", "");
            if (tokens[i + 1].get_type() == LITERAL_STRING) {
                arg = Variable(arg_names[arg_index], "word", arg_value);
            } else if (tokens[i + 1].get_type() == LITERAL_NUMBER) {
                arg = Variable(arg_names[arg_index], "lemon", arg_value);
            } else if (tokens[i + 1].get_type() == IDENTIFIER) {
                arg = Variable(arg_names[arg_index], this->interpreter->get_global_scope()->get_variable(tokens[i + 1].get_value()).get_type(), arg_value);
            }
            args.push_back(arg);
            arg_index++;
            arg_tokens = vector<Token>();
        } else {
            arg_tokens.push_back(token);
        }
    }

    // debug
    // for (Variable arg : args) {
    //     cout << arg.get_name() << " " << arg.get_value() << endl;
    // }
    // for (FunctionSymlink symlink : symlinks) {
    //     cout << symlink.get_name() << " " << symlink.get_original_name() << endl;
    // }

    // resolve function
    Variable output = function.evaluate(args, this->interpreter->get_functions(), symlinks);
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