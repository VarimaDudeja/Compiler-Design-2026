#ifndef AST_H
#define AST_H

typedef enum {
    NODE_DECLARE,
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_INPUT,
    NODE_IF,
    NODE_WHILE,
    NODE_NUM,
    NODE_IDENTIFIER,
    NODE_OP,
    NODE_SEQ,
    NODE_EXPR_STMT
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char name[64];
    int value;
    char op;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *else_body;
} ASTNode;

ASTNode *create_num(int value);
ASTNode *create_identifier(char *name);
ASTNode *create_op(char op, ASTNode *left, ASTNode *right);
ASTNode *create_declare(char *name, ASTNode *expr);
ASTNode *create_assign(char *name, ASTNode *expr);
ASTNode *create_print(ASTNode *expr);
ASTNode *create_input(char *name);
ASTNode *create_if(ASTNode *cond, ASTNode *if_body, ASTNode *else_body);
ASTNode *create_while(ASTNode *cond, ASTNode *body);
ASTNode *create_seq(ASTNode *left, ASTNode *right);
ASTNode *create_expr_stmt(ASTNode *expr);

#endif
