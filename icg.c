#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "icg.h"

#define MAX_VARS 50
char varNames[MAX_VARS][20];
int  varValues[MAX_VARS];
int  varCount = 0;

void setVar(char *name, int val) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(varNames[i], name) == 0) {
            varValues[i] = val;
            return;
        }
    }
    strcpy(varNames[varCount], name);
    varValues[varCount++] = val;
}

int getVar(char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(varNames[i], name) == 0)
            return varValues[i];
    }
    return 0;
}

int tempCount = 0;

void newTemp(char *buf) {
    sprintf(buf, "t%d", tempCount++);
}

void generateExpr(ASTNode *node, char *result) {

    if (node->type == NODE_NUMBER) {
        newTemp(result);
        printf("  %s = %d\n", result, node->value);
        setVar(result, node->value);
    }
    else if (node->type == NODE_IDENTIFIER) {
        strcpy(result, node->name);
    }
    else if (node->type == NODE_BINARY_OP) {
        char left[20], right[20];
        generateExpr(node->left, left);
        generateExpr(node->right, right);
        newTemp(result);
        int lval = getVar(left);
        int rval = getVar(right);
        int res  = node->op=='+' ? lval + rval :
                   node->op=='-' ? lval - rval :
                   node->op=='*' ? lval * rval : lval / rval;
        printf("  %s = %s %c %s\n", result, left, node->op, right);
        setVar(result, res);
    }
}

void generateICG(ASTNode *node) {
    if (!node) return;
    char temp[20];

    switch (node->type) {
        case NODE_DECLARATION:
            printf("  declare %s\n", node->name);
            if (node->left) {
                generateExpr(node->left, temp);
                printf("  %s = %s\n", node->name, temp);
                setVar(node->name, getVar(temp));
            }
            break;
        case NODE_ASSIGNMENT:
            generateExpr(node->left, temp);
            printf("  %s = %s\n", node->name, temp);
            setVar(node->name, getVar(temp));
            break;
        case NODE_PRINT:
            printf("  print %s = %d\n", node->name, getVar(node->name));
            break;
        default: break;
    }
}