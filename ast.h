#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_PRINT,
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_BINARY_OP
} NodeType;

typedef struct ASTNode {
    NodeType type;

    char *name;        // for identifiers
    int value;         // for numbers
    char op;           // for + or -

    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode* createNumberNode(int value);
ASTNode* createIdentifierNode(char* name);
ASTNode* createBinaryNode(char op, ASTNode* left, ASTNode* right);
ASTNode* createDeclarationNode(char* name, ASTNode* expr);
ASTNode* createAssignmentNode(char* name, ASTNode* expr);
ASTNode* createPrintNode(char* name);

void printAST(ASTNode* node, int level);

#endif