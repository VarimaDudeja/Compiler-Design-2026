#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "icg.h"

/* ── Temp variable counter ───────────────────────────────── */
static int tempCount = 0;

static char* newTemp() {
    char* temp = malloc(10);
    sprintf(temp, "t%d", tempCount++);
    return temp;
}

/* ── Main ICG function (recursive) ──────────────────────── */
char* generateICG(ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {

    /* ── NUMBER  →  just return the value as string ───────── */
    case NODE_NUMBER: {
        char* val = malloc(20);
        sprintf(val, "%d", node->value);
        return val;
    }

    /* ── IDENTIFIER  →  return its name ──────────────────── */
    case NODE_IDENTIFIER: {
        return strdup(node->name);
    }

    /* ── BINARY OP  →  t1 = left op right ────────────────── */
    case NODE_BINARY_OP: {
        char* left  = generateICG(node->left);
        char* right = generateICG(node->right);
        char* temp  = newTemp();
        printf("%s = %s %c %s\n", temp, left, node->op, right);
        free(left);
        free(right);
        return temp;
    }

    /* ── DECLARATION  →  evaluate expr, assign to variable ── */
    case NODE_DECLARATION: {
        char* expr = generateICG(node->left);
        printf("%s = %s\n", node->name, expr);
        free(expr);
        return strdup(node->name);
    }

    /* ── ASSIGNMENT  →  evaluate expr, assign to variable ─── */
    case NODE_ASSIGNMENT: {
        char* expr = generateICG(node->left);
        printf("%s = %s\n", node->name, expr);
        free(expr);
        return strdup(node->name);
    }

    /* ── PRINT  →  print variable ─────────────────────────── */
    case NODE_PRINT: {
        printf("PRINT %s\n", node->name);
        return NULL;
    }

    default:
        return NULL;
    }
}
