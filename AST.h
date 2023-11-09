#pragma once

//
// Created by plusleft on 11/8/2023.
//

#include "Token.h"

class ASTNode {
public:
    Token token;
    ASTNode* parent;
    ASTNode* left;
    ASTNode* right;

    void set_parent(ASTNode* parent);
    void set_left(ASTNode* left);
    void set_right(ASTNode* right);
};