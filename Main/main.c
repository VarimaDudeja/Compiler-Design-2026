#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"
#include "icg.h"

ASTNode* root;
extern FILE* yyin;
extern int yyparse();

char* optimizeCode(char* input);
char* generateTargetCode(char* input);

int main() {
    yyin = fopen("input.vl", "r");
    if (!yyin) {
        perror("Error opening input.vl");
        return 1;
    }

    root = NULL;
    if (yyparse() == 0 && root) {
        initSymbolTable();
        AnnotatedNode* annotated = analyseAST(root);
        if (getSemanticErrorCount() == 0) {
            char* icgCode = generateICG(root);
            char* optimized = optimizeCode(icgCode);
            char* target = generateTargetCode(optimized);

            // Create full C program
            char* fullCode = malloc(strlen(target) + 100);
            sprintf(fullCode, "#include <stdio.h>\nint main(){\n%sreturn 0;\n}", target);

            FILE* f = fopen("output.c", "w");
            if (f) {
                fprintf(f, "%s", fullCode);
                fclose(f);
                printf("Output generated in output.c\n");
            } else {
                perror("Error writing output.c");
            }

            free(fullCode);
            free(target);
        }
    }

    fclose(yyin);
    return 0;
}