#pragma once

//
// Created by plusleft on 11/6/2023.
//

#include <vector>
#include <string>

using namespace std;

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
};

class Scope {
private:
    vector<Variable> variables;
public:
    Scope() = default;

    void add_variable(Variable variable) {
        this->variables.push_back(variable);
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

class Interpreter {
private:
    vector<Scope> scopes;
public:
    Interpreter() = default;

    void add_scope(Scope scope) {
        this->scopes.push_back(scope);
    }

    Scope get_scope(int index) {
        return this->scopes[index];
    }

    Variable get_variable(string name) {
        for (int i = this->scopes.size() - 1; i >= 0; i--) {
            Scope scope = this->scopes[i];
            Variable variable = scope.get_variable(name);
            if (variable.get_name() != "") {
                return variable;
            }
        }
        return Variable("", "", "");
    }
};