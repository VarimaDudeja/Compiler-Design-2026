#include <stdio.h>
#include "ast.h"
#include "semantic.h"

extern int yyparse(void);
extern FILE *yyin;
extern ASTNode *root;

void generate_code(ASTNode *root);

int main(void) {
    yyin = fopen("input.vl", "r");
    if (!yyin) {
        perror("Error opening input.vl");
        return 1;
    }

    root = NULL;
    if (yyparse() != 0 || root == NULL) {
        fclose(yyin);
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    semantic_check(root);
    if (semantic_has_errors()) {
        fclose(yyin);
        fprintf(stderr, "Compilation stopped due to semantic errors\n");
        return 1;
    }

    generate_code(root);
    fclose(yyin);
    return 0;
}
