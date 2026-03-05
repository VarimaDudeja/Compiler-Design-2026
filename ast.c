#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* createNumberNode(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

ASTNode* createIdentifierNode(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->name = strdup(name);
    node->left = node->right = NULL;
    return node;
}

ASTNode* createBinaryNode(char op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BINARY_OP;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* createDeclarationNode(char* name, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_DECLARATION;
    node->name = strdup(name);
    node->left = expr;
    return node;
}

ASTNode* createAssignmentNode(char* name, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGNMENT;
    node->name = strdup(name);
    node->left = expr;
    return node;
}

ASTNode* createPrintNode(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PRINT;
    node->name = strdup(name);
    return node;
}

void printIndent(int level) {
    for(int i = 0; i < level; i++)
        printf("  ");
}

void printAST(ASTNode* node, int level) {
    if (!node) return;

    printIndent(level);

    switch(node->type) {
        case NODE_DECLARATION:
            printf("Declaration: %s\n", node->name);
            printAST(node->left, level + 1);
            break;

        case NODE_ASSIGNMENT:
            printf("Assignment: %s\n", node->name);
            printAST(node->left, level + 1);
            break;

        case NODE_PRINT:
            printf("Print: %s\n", node->name);
            break;

        case NODE_NUMBER:
            printf("Number: %d\n", node->value);
            break;

        case NODE_IDENTIFIER:
            printf("Identifier: %s\n", node->name);
            break;

        case NODE_BINARY_OP:
            printf("Operator: %c\n", node->op);
            printAST(node->left, level + 1);
            printAST(node->right, level + 1);
            break;

        default:
            break;
    }
}