#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "icg.h"
#include "semantic.h"

extern int yyparse(void);
extern FILE *yyin;
extern ASTNode *root;

char *optimizeCode(char *input);
char *generateTargetCode(char *input);

int main() {
    char *icgCode;
    char *optimizedCode;
    char *targetCode;
    FILE *out;

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

    icgCode = generateICG(root);
    optimizedCode = optimizeCode(icgCode);
    targetCode = generateTargetCode(optimizedCode);

    out = fopen("output.c", "w");
    if (!out) {
        perror("Error opening output.c");
        free(icgCode);
        free(optimizedCode);
        free(targetCode);
        fclose(yyin);
        return 1;
    }

    fprintf(out, "#include <stdio.h>\n\n");
    fprintf(out, "int main() {\n");
    fprintf(out, "%s", targetCode);
    fprintf(out, "    return 0;\n");
    fprintf(out, "}\n");
    fclose(out);

    free(icgCode);
    free(optimizedCode);
    free(targetCode);
    printf("Generated output.c\n");
    fclose(yyin);
    return 0;
}
