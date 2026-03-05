%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

void yyerror(const char *s);
int yylex();
%}
%code requires {
    #include "ast.h"
}
/* ---------- UNION ---------- */
%union {
    int num;
    char *str;
    ASTNode* node;
}

/* ---------- TOKENS ---------- */
%token DECLARE SET PRINT AS INT VALUE
%token <str> IDENTIFIER
%token <num> NUMBER

/* ---------- TYPE DECLARATIONS ---------- */
%type <node> program statement_list statement
%type <node> declaration assignment print_stmt
%type <node> expression term

%%

/* ---------- GRAMMAR RULES ---------- */

program:
    statement_list
    {
        printf("Parsing Completed Successfully\n");
    }
;

/* Multiple statements support */
statement_list:
      statement
        { $$ = $1; }
    | statement_list statement
        { $$ = $2; }
;

statement:
      declaration
        { $$ = $1; }
    | assignment
        { $$ = $1; }
    | print_stmt
        { $$ = $1; }
;

/* -------- DECLARATION -------- */
declaration:
    DECLARE IDENTIFIER AS INT VALUE expression
    {
        $$ = createDeclarationNode($2, $6);
        printf("\n--- AST ---\n");
        printAST($$, 0);
    }
;

/* -------- ASSIGNMENT -------- */
assignment:
    SET IDENTIFIER '=' expression
    {
        $$ = createAssignmentNode($2, $4);
        printf("\n--- AST ---\n");
        printAST($$, 0);
    }
;

/* -------- PRINT -------- */
print_stmt:
    PRINT IDENTIFIER
    {
        $$ = createPrintNode($2);
        printf("\n--- AST ---\n");
        printAST($$, 0);
    }
;

/* -------- EXPRESSIONS -------- */
expression:
      expression '+' term
        { $$ = createBinaryNode('+', $1, $3); }
    | expression '-' term
        { $$ = createBinaryNode('-', $1, $3); }
    | term
        { $$ = $1; }
;

term:
      NUMBER
        { $$ = createNumberNode($1); }
    | IDENTIFIER
        { $$ = createIdentifierNode($1); }
;

%%

/* ---------- MAIN FUNCTION ---------- */
int main() {
    return yyparse();
}

/* ---------- ERROR FUNCTION ---------- */
void yyerror(const char *s) {
    printf("Syntax Error: %s\n", s);

}
