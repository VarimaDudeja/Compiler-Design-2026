%{
#include <stdio.h>
#include "ast.h"

ASTNode *root;

int yylex(void);
void yyerror(const char *s);
%}

%union {
    int num;
    char *str;
    ASTNode *node;
}

%token DECLARE SET PRINT INPUT FROM USER VALUE IF ELSE ELSEIF WHILE FOR STEP THEN ENDIF END STOP DONE TO
%token EQ NE
%token <str> IDENTIFIER
%token <num> NUMBER

%nonassoc '<' '>' EQ NE
%left '+' '-'
%left '*' '/' '%'

%type <node> program stmt_list stmt declaration assignment print_stmt input_stmt if_stmt while_stmt for_stmt loop_init if_tail loop_tail expr

%%

program:
    stmt_list { root = $1; }
;

stmt_list:
      stmt { $$ = $1; }
    | stmt_list stmt { $$ = create_seq($1, $2); }
;

stmt:
      declaration { $$ = $1; }
    | assignment { $$ = $1; }
    | print_stmt { $$ = $1; }
    | input_stmt { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | expr { $$ = create_expr_stmt($1); }
;

declaration:
      DECLARE IDENTIFIER { $$ = create_declare($2, NULL); }
    | DECLARE IDENTIFIER VALUE expr { $$ = create_declare($2, $4); }
;

assignment:
      SET IDENTIFIER '=' expr { $$ = create_assign($2, $4); }
    | SET IDENTIFIER TO expr { $$ = create_assign($2, $4); }
;

print_stmt:
    PRINT expr { $$ = create_print($2); }
;

input_stmt:
      INPUT IDENTIFIER { $$ = create_input($2); }
    | INPUT IDENTIFIER FROM USER { $$ = create_input($2); }
;

if_stmt:
      IF expr THEN stmt_list if_tail { $$ = create_if($2, $4, $5); }
;

while_stmt:
      WHILE expr THEN stmt_list loop_tail { $$ = create_while($2, $4); }
;

for_stmt:
      FOR loop_init WHILE expr STEP assignment THEN stmt_list loop_tail
        { $$ = create_seq($2, create_while($4, create_seq($8, $6))); }
    | FOR loop_init WHILE expr STEP assignment stmt_list loop_tail
        { $$ = create_seq($2, create_while($4, create_seq($7, $6))); }
;

loop_init:
      declaration { $$ = $1; }
    | assignment { $$ = $1; }
;

if_tail:
      ENDIF { $$ = NULL; }
    | END IF { $$ = NULL; }
    | STOP { $$ = NULL; }
    | DONE { $$ = NULL; }
    | ELSE stmt_list ENDIF { $$ = $2; }
    | ELSE stmt_list END IF { $$ = $2; }
    | ELSE stmt_list STOP { $$ = $2; }
    | ELSE stmt_list DONE { $$ = $2; }
    | ELSEIF expr THEN stmt_list if_tail { $$ = create_if($2, $4, $5); }
;

loop_tail:
      ENDIF { $$ = NULL; }
    | END IF { $$ = NULL; }
    | STOP { $$ = NULL; }
    | DONE { $$ = NULL; }
;

expr:
      expr '+' expr { $$ = create_op('+', $1, $3); }
    | expr '-' expr { $$ = create_op('-', $1, $3); }
    | expr '*' expr { $$ = create_op('*', $1, $3); }
    | expr '/' expr { $$ = create_op('/', $1, $3); }
    | expr '%' expr { $$ = create_op('%', $1, $3); }
    | expr '<' expr { $$ = create_op('<', $1, $3); }
    | expr '>' expr { $$ = create_op('>', $1, $3); }
    | expr EQ expr { $$ = create_op('=', $1, $3); }
    | expr NE expr { $$ = create_op('!', $1, $3); }
    | '(' expr ')' { $$ = $2; }
    | NUMBER { $$ = create_num($1); }
    | IDENTIFIER { $$ = create_identifier($1); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
