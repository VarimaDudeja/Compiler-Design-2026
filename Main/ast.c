#include <stdlib.h>
#include <string.h>
#include "ast.h"

static ASTNode *newNode(NodeType type) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = type;
    return node;
}

ASTNode *create_num(int value) {
    ASTNode *node = newNode(NODE_NUM);
    node->value = value;
    return node;
}

ASTNode *create_identifier(char *name) {
    ASTNode *node = newNode(NODE_IDENTIFIER);
    strncpy(node->name, name, sizeof(node->name) - 1);
    return node;
}

ASTNode *create_op(char op, ASTNode *left, ASTNode *right) {
    ASTNode *node = newNode(NODE_OP);
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *create_declare(char *name, ASTNode *expr) {
    ASTNode *node = newNode(NODE_DECLARE);
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->left = expr;
    return node;
}

ASTNode *create_assign(char *name, ASTNode *expr) {
    ASTNode *node = newNode(NODE_ASSIGN);
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->left = expr;
    return node;
}

ASTNode *create_print(ASTNode *expr) {
    ASTNode *node = newNode(NODE_PRINT);
    node->left = expr;
    return node;
}

ASTNode *create_input(char *name) {
    ASTNode *node = newNode(NODE_INPUT);
    strncpy(node->name, name, sizeof(node->name) - 1);
    return node;
}

ASTNode *create_if(ASTNode *cond, ASTNode *if_body, ASTNode *else_body) {
    ASTNode *node = newNode(NODE_IF);
    node->left = cond;
    node->right = if_body;
    node->else_body = else_body;
    return node;
}

ASTNode *create_while(ASTNode *cond, ASTNode *body) {
    ASTNode *node = newNode(NODE_WHILE);
    node->left = cond;
    node->right = body;
    return node;
}

ASTNode *create_seq(ASTNode *left, ASTNode *right) {
    ASTNode *node = newNode(NODE_SEQ);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *create_expr_stmt(ASTNode *expr) {
    ASTNode *node = newNode(NODE_EXPR_STMT);
    node->left = expr;
    return node;
}
