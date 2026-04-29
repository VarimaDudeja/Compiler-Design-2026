#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "icg.h"

extern ASTNode* root;
extern int yylex();
extern int yyparse();
extern FILE* yyin;

int main() {
    yyin = fopen("input.vl", "r");
    if (!yyin) {
        perror("Error opening input.vl");
        return 1;
    }

    root = NULL;
    if (yyparse() == 0 && root) {
        char* icgCode = generateICG(root);
        printf("=== GENERATED ICG CODE ===\n%s\n=== END ICG ===\n", icgCode);
    }

    fclose(yyin);
    return 0;
}
