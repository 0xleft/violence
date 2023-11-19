#pragma once

//
// Created by plusleft on 11/6/2023.
//

#include <vector>
#include <string>
#include <iostream>
#include "Token.h"

using namespace std;

class Interpreter;
class Variable;
class Function;
class Scope;
class FunctionSymlink;

class Variable {
private:
    string name;
    string type;
    string value;
public:
    Variable(string name, string type, string value) {
        this->name = name;
        this->type = type;
        this->value = value;
    }

    string get_name() {
        return this->name;
    }

    string get_type() {
        return this->type;
    }

    string get_value() {
        return this->value;
    }

    string to_string() {
        return "Variable(" + this->name + ", " + this->type + ", " + this->value + ")";
    }
};

class Function {
private:
    string name;
    vector<string> arg_names;
    int arg_count;
    string return_type;
    vector<Token> body;

public:
    Function(string name, vector<string> arg_names, vector<Token> body, string return_type) {
        this->name = name;
        this->arg_names = arg_names;
        this->arg_count = arg_names.size();
        this->body = body;
        this->return_type = return_type;
    }

    Function(string name, int arg_count, vector<Token> body, string return_type) {
        this->name = name;
        this->arg_count = arg_count;
        for (int i = 0; i < arg_count; i++) {
            this->arg_names.push_back("arg" + to_string(i));
        }
        this->body = body;
        this->return_type = return_type;
    }

    int get_arg_count() {
        return this->arg_count;
    }

    vector<string> get_arg_names() {
        return this->arg_names;
    }

    vector<Token> get_body() {
        return this->body;
    }

    string get_return_type() {
        return this->return_type;
    }

    string get_name() {
        return this->name;
    }

    Variable evaluate(vector<Variable> args, vector<Function> functions, vector<FunctionSymlink> function_symlinks, Interpreter *interpreter);
};

class Scope {
private:
    vector<Variable> variables;
public:
    Scope() {
        this->variables = vector<Variable>();
    }

    void set_variable(Variable variable) {
        // check if variable exists
        if (this->get_variable(variable.get_name()).get_name() == "") {
            this->variables.push_back(variable);
            return;
        }

        // if it does, replace it
        for (int i = 0; i < this->variables.size(); i++) {
            if (this->variables[i].get_name() == variable.get_name()) {
                this->variables[i] = variable;
                return;
            }
        }
    }

    void print_variable_count() {
        cout << "variable count: " << this->variables.size() << endl;
    }

    void print_variables() {
        for (Variable variable : this->variables) {
            cout << variable.get_name() << "->type->" << variable.get_type() << "->value->" << variable.get_value() << endl;
        }
    }

    Variable get_variable(string name) {
        for (Variable variable : this->variables) {
            if (variable.get_name() == name) {
                return variable;
            }
        }
        return Variable("", "", "");
    }
};

// make more useful
class FunctionSymlink {
private:
    string name;
    string original_name;

public:
    FunctionSymlink(string name, string original_name) {
        this->name = name;
        this->original_name = original_name;
    }

    string get_name() {
        return this->name;
    }

    string get_original_name() {
        return this->original_name;
    }
};

class CFunction {
private:
    string filename;
    string function_name;
    string return_type;
public:
    CFunction(string filename, string function_name, string return_type) {
        this->filename = filename;
        this->function_name = function_name;
        this->return_type = return_type;
    }

    string get_filename() {
        return this->filename;
    }

    string get_name() {
        return this->function_name;
    }

    string get_return_type() {
        return this->return_type;
    }
};

class Interpreter {
private:
    Scope *global_scope;
    vector<Function> functions;
    vector<CFunction> c_functions;
    vector<FunctionSymlink> function_symlinks;
    vector<string> imported;
    Interpreter *parent;
public:
    Interpreter();

    void add_c_function(CFunction c_function) {
        // check if variable with this name exits
        if (this->get_global_scope()->get_variable(c_function.get_name()).get_name() != "") {
            cout << "variable with name " << c_function.get_name() << " already exists while trying to name function after it :(" << endl;
            exit(1);
        }

        // check if function exists
        if (this->get_c_function(c_function.get_name()).get_name() == "") {
            this->c_functions.push_back(c_function);
            return;
        }

        // if it does, replace it
        for (int i = 0; i < this->c_functions.size(); i++) {
            if (this->c_functions[i].get_name() == c_function.get_name()) {
                this->c_functions[i] = c_function;
                return;
            }
        }
    }

    CFunction get_c_function(string name) {
        for (CFunction c_function : this->c_functions) {
            if (c_function.get_name() == name) {
                return c_function;
            }
        }
        return CFunction("", "", "");
    }

    vector<CFunction> get_c_functions() {
        return this->c_functions;
    }

    void set_parent(Interpreter *parent) {
        this->parent = parent;
    }

    Interpreter *get_parent() {
        return this->parent;
    }

    // import
    void add_imported(string name) {
        this->imported.push_back(name);
    }

    vector<string> get_imported() {
        return this->imported;
    }

    bool is_imported(string name) {
        for (string imported : this->imported) {
            if (imported == name) {
                return true;
            }
        }
        return false;
    }

    Scope *get_global_scope() {
        return this->global_scope;
    }

    vector<Function> get_functions() {
        return this->functions;
    }

    void set_function_symlink(FunctionSymlink function_symlink) {
        // check if function exists
        if (this->get_function(function_symlink.get_name()).get_name() != "") {
            cout << "function with name " << function_symlink.get_name() << " already exists while trying to name function after it :(" << endl;
            exit(1);
        }

        // check if function symlink exists
        if (this->get_function_symlink(function_symlink.get_name()).get_name() == "") {
            this->function_symlinks.push_back(function_symlink);
            return;
        }

        // if it does, replace it
        for (int i = 0; i < this->function_symlinks.size(); i++) {
            if (this->function_symlinks[i].get_name() == function_symlink.get_name()) {
                this->function_symlinks[i] = function_symlink;
                return;
            }
        }
    }

    FunctionSymlink get_function_symlink(string name) {
        for (FunctionSymlink function_symlink : this->function_symlinks) {
            if (function_symlink.get_name() == name) {
                return function_symlink;
            }
        }
        return FunctionSymlink("", "");
    }

    // functions
    void add_function(Function function) {
        // check if variable with this name exits
        if (this->get_global_scope()->get_variable(function.get_name()).get_name() != "") {
            cout << "variable with name " << function.get_name() << " already exists while trying to name function after it :(" << endl;
            exit(1);
        }

        // check if function exists
        if (this->get_function(function.get_name()).get_name() == "") {
            this->functions.push_back(function);
            return;
        }

        // if it does, replace it
        for (int i = 0; i < this->functions.size(); i++) {
            if (this->functions[i].get_name() == function.get_name()) {
                this->functions[i] = function;
                return;
            }
        }
    }

    Function get_function(string name) {
        for (Function function : this->functions) {
            if (function.get_name() == name) {
                return function;
            }
        }

        // check if function symlink exists
        FunctionSymlink function_symlink = this->get_function_symlink(name);
        if (function_symlink.get_name() != "") {
            return this->get_function(function_symlink.get_original_name());
        }

        return Function("", 0, vector<Token>(), "void");
    }

    vector<FunctionSymlink> get_symlinks() {
        return this->function_symlinks;
    }

    ~Interpreter() {
        delete this->global_scope;
    }
};

class Expression {
private:
    vector<Token> tokens;
    string type;
    Interpreter *interpreter;

public:
    Expression() = default;

    Expression(vector<Token> tokens, string type, Interpreter *interpreter) {
        this->tokens = tokens;
        this->type = type;
        this->interpreter = interpreter;
    }

    string resolve_function(vector<Token> tokens);
    string evaluate(string return_type);
    string resolve(Token token);
};