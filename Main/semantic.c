#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

typedef struct Symbol {
    char name[64];
    int initialized;
    struct Symbol *next;
} Symbol;

static Symbol *symbols = NULL;
static int semanticErrors = 0;

static void reset_symbols(void) {
    Symbol *current = symbols;
    while (current) {
        Symbol *next = current->next;
        free(current);
        current = next;
    }
    symbols = NULL;
}

static Symbol *lookup_symbol(const char *name) {
    for (Symbol *symbol = symbols; symbol; symbol = symbol->next) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol;
        }
    }
    return NULL;
}

static Symbol *insert_symbol(const char *name) {
    Symbol *symbol = calloc(1, sizeof(Symbol));
    strncpy(symbol->name, name, sizeof(symbol->name) - 1);
    symbol->next = symbols;
    symbols = symbol;
    return symbol;
}

static void semantic_error(const char *message, const char *name) {
    if (name) {
        fprintf(stderr, "[Semantic Error] %s: %s\n", message, name);
    } else {
        fprintf(stderr, "[Semantic Error] %s\n", message);
    }
    semanticErrors++;
}

static void semantic_warning(const char *message, const char *name) {
    if (name) {
        fprintf(stderr, "[Semantic Warning] %s: %s\n", message, name);
    } else {
        fprintf(stderr, "[Semantic Warning] %s\n", message);
    }
}

static int eval_constant(ASTNode *node, int *value) {
    int leftValue;
    int rightValue;

    if (!node) {
        return 0;
    }

    if (node->type == NODE_NUM) {
        *value = node->value;
        return 1;
    }

    if (node->type != NODE_OP) {
        return 0;
    }

    if (!eval_constant(node->left, &leftValue) || !eval_constant(node->right, &rightValue)) {
        return 0;
    }

    switch (node->op) {
        case '+': *value = leftValue + rightValue; return 1;
        case '-': *value = leftValue - rightValue; return 1;
        case '*': *value = leftValue * rightValue; return 1;
        case '/':
            if (rightValue == 0) {
                semantic_error("Division by zero in constant expression", NULL);
                return 0;
            }
            *value = leftValue / rightValue;
            return 1;
        case '%':
            if (rightValue == 0) {
                semantic_error("Modulo by zero in constant expression", NULL);
                return 0;
            }
            *value = leftValue % rightValue;
            return 1;
        case '<': *value = leftValue < rightValue; return 1;
        case '>': *value = leftValue > rightValue; return 1;
        case '=': *value = leftValue == rightValue; return 1;
        case '!': *value = leftValue != rightValue; return 1;
        default:
            return 0;
    }
}

static ASTNode *constant_fold(ASTNode *node) {
    int value;

    if (!node) {
        return NULL;
    }

    node->left = constant_fold(node->left);
    node->right = constant_fold(node->right);
    node->else_body = constant_fold(node->else_body);

    if (node->type == NODE_OP && eval_constant(node, &value)) {
        node->type = NODE_NUM;
        node->value = value;
        node->op = 0;
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

static void check_expr(ASTNode *node) {
    int rightValue;

    if (!node) {
        return;
    }

    switch (node->type) {
        case NODE_NUM:
            return;

        case NODE_IDENTIFIER: {
            Symbol *symbol = lookup_symbol(node->name);
            if (!symbol) {
                semantic_error("Variable used before declaration", node->name);
                return;
            }
            if (!symbol->initialized) {
                semantic_warning("Variable used before initialization", node->name);
            }
            return;
        }

        case NODE_OP:
            check_expr(node->left);
            check_expr(node->right);
            if ((node->op == '/' || node->op == '%') &&
                eval_constant(node->right, &rightValue) && rightValue == 0) {
                semantic_error(node->op == '/' ? "Division by zero" : "Modulo by zero", NULL);
            }
            return;

        default:
            return;
    }
}

static void check_stmt(ASTNode *node) {
    if (!node) {
        return;
    }

    switch (node->type) {
        case NODE_SEQ:
            check_stmt(node->left);
            check_stmt(node->right);
            break;

        case NODE_DECLARE: {
            Symbol *symbol = lookup_symbol(node->name);
            if (symbol) {
                semantic_error("Redeclaration of variable", node->name);
                break;
            }
            symbol = insert_symbol(node->name);
            if (node->left) {
                check_expr(node->left);
                symbol->initialized = 1;
            }
            break;
        }

        case NODE_ASSIGN: {
            Symbol *symbol = lookup_symbol(node->name);
            if (!symbol) {
                semantic_error("Assignment to undeclared variable", node->name);
                break;
            }
            check_expr(node->left);
            symbol->initialized = 1;
            break;
        }

        case NODE_INPUT: {
            Symbol *symbol = lookup_symbol(node->name);
            if (!symbol) {
                semantic_error("Input into undeclared variable", node->name);
                break;
            }
            symbol->initialized = 1;
            break;
        }

        case NODE_PRINT:
            check_expr(node->left);
            break;

        case NODE_EXPR_STMT:
            check_expr(node->left);
            break;

        case NODE_IF:
            check_expr(node->left);
            check_stmt(node->right);
            check_stmt(node->else_body);
            break;

        case NODE_WHILE:
            check_expr(node->left);
            check_stmt(node->right);
            break;

        default:
            break;
    }
}

void semantic_check(ASTNode *root) {
    semanticErrors = 0;
    reset_symbols();
    constant_fold(root);
    check_stmt(root);
}

int semantic_has_errors(void) {
    return semanticErrors > 0;
}
