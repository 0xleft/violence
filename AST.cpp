//
// Created by adoma on 11/8/2023.
//

#include "AST.h"
#include <iostream>

void ASTNode::set_parent(ASTNode *parent) {
    this->parent = parent;
}

void ASTNode::set_left(ASTNode *left) {
    this->left = left;
}

void ASTNode::set_right(ASTNode *right) {
    this->right = right;
}