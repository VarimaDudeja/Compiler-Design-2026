#include <stdio.h>
#include "ast.h"

static FILE *outFile;
static void generate_expr(ASTNode *node);

static void write_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        fputs("    ", outFile);
    }
}

static void generate_stmt(ASTNode *node, int indent);

static void generate_if(ASTNode *node, int indent) {
    write_indent(indent);
    fprintf(outFile, "if (");
    generate_expr(node->left);
    fprintf(outFile, ") {\n");
    generate_stmt(node->right, indent + 1);
    write_indent(indent);
    fprintf(outFile, "}");
    if (node->else_body) {
        if (node->else_body->type == NODE_IF) {
            fprintf(outFile, " else ");
            generate_if(node->else_body, 0);
            return;
        }
        fprintf(outFile, " else {\n");
        generate_stmt(node->else_body, indent + 1);
        write_indent(indent);
        fprintf(outFile, "}");
    }
    fprintf(outFile, "\n");
}

static void generate_while(ASTNode *node, int indent) {
    write_indent(indent);
    fprintf(outFile, "while (");
    generate_expr(node->left);
    fprintf(outFile, ") {\n");
    generate_stmt(node->right, indent + 1);
    write_indent(indent);
    fprintf(outFile, "}\n");
}

static void generate_expr(ASTNode *node) {
    if (!node) {
        return;
    }

    switch (node->type) {
        case NODE_NUM:
            fprintf(outFile, "%d", node->value);
            break;

        case NODE_IDENTIFIER:
            fprintf(outFile, "%s", node->name);
            break;

        case NODE_OP:
            fprintf(outFile, "(");
            generate_expr(node->left);
            if (node->op == '=') {
                fprintf(outFile, " == ");
            } else if (node->op == '!') {
                fprintf(outFile, " != ");
            } else {
                fprintf(outFile, " %c ", node->op);
            }
            generate_expr(node->right);
            fprintf(outFile, ")");
            break;

        default:
            break;
    }
}

static void generate_stmt(ASTNode *node, int indent) {
    if (!node) {
        return;
    }

    switch (node->type) {
        case NODE_SEQ:
            generate_stmt(node->left, indent);
            generate_stmt(node->right, indent);
            break;

        case NODE_DECLARE:
            write_indent(indent);
            fprintf(outFile, "int %s", node->name);
            if (node->left) {
                fprintf(outFile, " = ");
                generate_expr(node->left);
            }
            fprintf(outFile, ";\n");
            break;

        case NODE_ASSIGN:
            write_indent(indent);
            fprintf(outFile, "%s = ", node->name);
            generate_expr(node->left);
            fprintf(outFile, ";\n");
            break;

        case NODE_PRINT:
            write_indent(indent);
            fprintf(outFile, "printf(\"%%d\\n\", ");
            generate_expr(node->left);
            fprintf(outFile, ");\n");
            break;

        case NODE_INPUT:
            write_indent(indent);
            fprintf(outFile, "scanf(\"%%d\", &%s);\n", node->name);
            break;

        case NODE_EXPR_STMT:
            write_indent(indent);
            generate_expr(node->left);
            fprintf(outFile, ";\n");
            break;

        case NODE_IF:
            generate_if(node, indent);
            break;

        case NODE_WHILE:
            generate_while(node, indent);
            break;

        default:
            break;
    }
}

void generate_code(ASTNode *root) {
    outFile = fopen("output.c", "w");
    if (!outFile) {
        perror("Error opening output.c");
        return;
    }

    fprintf(outFile, "#include <stdio.h>\n\n");
    fprintf(outFile, "int main(void) {\n");
    generate_stmt(root, 1);
    fprintf(outFile, "    return 0;\n");
    fprintf(outFile, "}\n");

    fclose(outFile);
    printf("Generated output.c\n");
}
