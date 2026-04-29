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

/* ── Buffer for code ─────────────────────────────────────── */
static char buffer[10000];
static int bufpos = 0;

static char* generateICGRec(ASTNode* node);

char* generateICG(ASTNode* node) {
    bufpos = 0;
    memset(buffer, 0, sizeof(buffer));
    tempCount = 0;
    generateICGRec(node);
    return buffer;
}

char* generateICGRec(ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {

    case NODE_PROGRAM:
        generateICGRec(node->left);
        return NULL;

    case NODE_SEQ:
        generateICGRec(node->left);
        generateICGRec(node->right);
        return NULL;

    /* ── NUMBER  →  just return the value as string ───────── */
    case NODE_NUM: {
        char* val = malloc(20);
        sprintf(val, "%d", node->value);
        return val;
    }

    /* ── IDENTIFIER  →  return its name ──────────────────── */
    case NODE_IDENTIFIER: {
        return strdup(node->name);
    }

    /* ── BINARY OP  →  t1 = left op right ────────────────── */
    case NODE_OP: {
        char* left  = generateICGRec(node->left);
        char* right = generateICGRec(node->right);
        int lval, rval;
        if (sscanf(left, "%d", &lval) == 1 && sscanf(right, "%d", &rval) == 1) {
            // Constant folding
            int result;
            switch (node->op) {
                case '+': result = lval + rval; break;
                case '-': result = lval - rval; break;
                case '*': result = lval * rval; break;
                case '/': if (rval != 0) result = lval / rval; else result = 0; break;
                default: result = 0;
            }
            char* res = malloc(20);
            sprintf(res, "%d", result);
            free(left);
            free(right);
            return res;
        } else {
            char* temp  = newTemp();
            bufpos += sprintf(buffer + bufpos, "int %s = %s %c %s\n", temp, left, node->op, right);
            free(left);
            free(right);
            return temp;
        }
    }

    /* ── DECLARATION  →  evaluate expr, assign to variable ── */
    case NODE_DECLARE: {
        if (node->left == NULL) {
            /* Uninitialized declaration */
            bufpos += sprintf(buffer + bufpos, "int %s\n", node->name);
        } else {
            char* expr = generateICGRec(node->left);
            bufpos += sprintf(buffer + bufpos, "int %s=%s\n", node->name, expr);
            if (expr) free(expr);
        }
        return strdup(node->name);
    }

    /* ── ASSIGNMENT  →  evaluate expr, assign to variable ─── */
    case NODE_ASSIGN: {
        char* expr = generateICGRec(node->left);
        bufpos += sprintf(buffer + bufpos, "%s=%s\n", node->name, expr);
        if (expr) free(expr);
        return strdup(node->name);
    }

    /* ── INPUT  →  read from user ────────────────────────── */
    case NODE_INPUT: {
        bufpos += sprintf(buffer + bufpos, "scanf(\"%%d\", &%s)\n", node->name);
        return NULL;
    }

    /* ── PRINT  →  print expression ─────────────────────────── */
    case NODE_PRINT: {
        char* expr = generateICGRec(node->left);
        bufpos += sprintf(buffer + bufpos, "printf(\"%%d\\\\n\", %s)\n", expr);
        if (expr) free(expr);
        return NULL;
    }

    /* ── IF / IF-ELSE ────────────────────────────────────────── */
    case NODE_IF: {
        static int ifcount = 0;
        int ifid = ifcount++;
        
        // Condition is cond_left op cond_right
        char* left = generateICGRec(node->cond_left);
        char* right = generateICGRec(node->cond_right);
        bufpos += sprintf(buffer + bufpos, "if (%s > %s) {\n", left, right);
        
        /* Generate THEN branch */
        generateICGRec(node->if_body);
        
        /* Generate ELSE branch if present */
        if (node->else_body) {
            bufpos += sprintf(buffer + bufpos, "} else {\n");
            generateICGRec(node->else_body);
        }
        
        bufpos += sprintf(buffer + bufpos, "}\n");
        if (left) free(left);
        if (right) free(right);
        return NULL;
    }

    default:
        return NULL;
    }
}
