%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern ASTNode* root;
void yyerror(const char *s);
int yylex();
%}

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
%type <node> expression term factor

%%

/* ---------- GRAMMAR RULES ---------- */

program:
    statement_list
    {
        root = createProgramNode($1);
        printf("Parsing Completed Successfully\n");
    }
;

/* Multiple statements support */
statement_list:
      statement
        { $$ = $1; }
    | statement_list statement
        { $$ = createStatementListNode($1, $2); }
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
    }
;

/* -------- ASSIGNMENT -------- */
assignment:
    SET IDENTIFIER '=' expression
    {
        $$ = createAssignmentNode($2, $4);
    }
;

/* -------- PRINT -------- */
print_stmt:
    PRINT expression
    {
        $$ = createPrintNode($2);
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
      term '*' factor
        { $$ = createBinaryNode('*', $1, $3); }
    | term '/' factor
        { $$ = createBinaryNode('/', $1, $3); }
    | factor
        { $$ = $1; }
;

factor:
      NUMBER
        { $$ = createNumberNode($1); }
    | IDENTIFIER
        { $$ = createIdentifierNode($1); }
;

%%

/* ---------- ERROR FUNCTION ---------- */
void yyerror(const char *s) {
    printf("Syntax Error: %s\n", s);

}
