%{
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"
%}

%%

"DECLARE"      { return DECLARE; }
"SET"          { return SET; }
"TO"           { return TO; }
"PRINT"        { return PRINT; }
"INPUT"        { return INPUT; }
"FROM"         { return FROM; }
"USER"         { return USER; }
"VALUE"        { return VALUE; }
"IF"           { return IF; }
"WHILE"        { return WHILE; }
"FOR"          { return FOR; }
"STEP"         { return STEP; }
"THEN"         { return THEN; }
"ELSEIF"       { return ELSEIF; }
"ELSE"         { return ELSE; }
"ENDIF"        { return ENDIF; }
"END"          { return END; }
"STOP"         { return STOP; }
"DONE"         { return DONE; }

"=="           { return EQ; }
"!="           { return NE; }
[0-9]+         { yylval.num = atoi(yytext); return NUMBER; }
[A-Za-z_][A-Za-z0-9_]* {
                  yylval.str = strdup(yytext);
                  return IDENTIFIER;
               }

"="            { return '='; }
"+"            { return '+'; }
"-"            { return '-'; }
"*"            { return '*'; }
"/"            { return '/'; }
"%"            { return '%'; }
"<"            { return '<'; }
">"            { return '>'; }
"("            { return '('; }
")"            { return ')'; }

[ \t\r\n]+     { /* ignore whitespace */ }
.              { fprintf(stderr, "Unknown character: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}
