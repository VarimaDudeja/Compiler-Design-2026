#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "icg.h"

static char buffer[20000];
static int bufpos = 0;
static int tempCount = 0;

static void emit_line(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    bufpos += vsnprintf(buffer + bufpos, sizeof(buffer) - bufpos, fmt, args);
    va_end(args);
}

static char *dup_string(const char *value) {
    char *copy = malloc(strlen(value) + 1);
    strcpy(copy, value);
    return copy;
}

static char *new_temp(void) {
    char temp[32];
    snprintf(temp, sizeof(temp), "t%d", tempCount++);
    return dup_string(temp);
}

static int is_simple_expr(ASTNode *node) {
    return node && (node->type == NODE_NUM || node->type == NODE_IDENTIFIER);
}

static const char *op_text(char op) {
    switch (op) {
        case '=': return "==";
        case '!': return "!=";
        case '+': return "+";
        case '-': return "-";
        case '*': return "*";
        case '/': return "/";
        case '%': return "%";
        case '<': return "<";
        case '>': return ">";
        default: return "";
    }
}

static char *format_expr(ASTNode *node, int wrap_self) {
    char expr[256];
    char *left;
    char *right;

    if (!node) {
        return NULL;
    }

    switch (node->type) {
        case NODE_NUM:
            snprintf(expr, sizeof(expr), "%d", node->value);
            return dup_string(expr);

        case NODE_IDENTIFIER:
            return dup_string(node->name);

        case NODE_OP:
            left = format_expr(node->left, 1);
            right = format_expr(node->right, 1);
            if (wrap_self) {
                snprintf(expr, sizeof(expr), "(%s %s %s)", left, op_text(node->op), right);
            } else {
                snprintf(expr, sizeof(expr), "%s %s %s", left, op_text(node->op), right);
            }
            free(left);
            free(right);
            return dup_string(expr);

        default:
            return NULL;
    }
}

static char *emit_expr(ASTNode *node) {
    char *expr;
    char *temp;

    if (!node) {
        return NULL;
    }

    if (node->type != NODE_OP) {
        return format_expr(node, 0);
    }

    expr = format_expr(node, 1);

    if (is_simple_expr(node->left) && is_simple_expr(node->right)) {
        return expr;
    }

    temp = new_temp();
    emit_line("TMP %s = %s\n", temp, expr);
    free(expr);
    return temp;
}

static void emit_stmt(ASTNode *node) {
    char *expr;

    if (!node) {
        return;
    }

    switch (node->type) {
        case NODE_SEQ:
            emit_stmt(node->left);
            emit_stmt(node->right);
            break;

        case NODE_DECLARE:
            if (node->left) {
                expr = emit_expr(node->left);
                emit_line("DECLSET %s = %s\n", node->name, expr);
                free(expr);
            } else {
                emit_line("DECL %s\n", node->name);
            }
            break;

        case NODE_ASSIGN:
            expr = emit_expr(node->left);
            emit_line("SET %s = %s\n", node->name, expr);
            free(expr);
            break;

        case NODE_PRINT:
            expr = emit_expr(node->left);
            emit_line("PRINT %s\n", expr);
            free(expr);
            break;

        case NODE_INPUT:
            emit_line("INPUT %s\n", node->name);
            break;

        case NODE_IF:
            expr = format_expr(node->left, 0);
            emit_line("IF %s\n", expr);
            free(expr);
            emit_stmt(node->right);
            if (node->else_body) {
                emit_line("ELSE\n");
                emit_stmt(node->else_body);
            }
            emit_line("ENDIF\n");
            break;

        case NODE_WHILE:
            expr = format_expr(node->left, 0);
            emit_line("WHILE %s\n", expr);
            free(expr);
            emit_stmt(node->right);
            emit_line("ENDWHILE\n");
            break;

        case NODE_EXPR_STMT:
            expr = emit_expr(node->left);
            emit_line("EXPR %s\n", expr);
            free(expr);
            break;

        default:
            break;
    }
}

char *generateICG(ASTNode *node) {
    bufpos = 0;
    buffer[0] = '\0';
    tempCount = 0;
    emit_stmt(node);
    return dup_string(buffer);
}
