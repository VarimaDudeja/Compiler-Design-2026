%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%

"DECLARE"      { return DECLARE; }
"SET"          { return SET; }
"PRINT"        { return PRINT; }
"AS"           { return AS; }
"INT"          { return INT; }
"VALUE"        { return VALUE; }

[0-9]+         {
                  yylval.num = atoi(yytext);
                  return NUMBER;
               }

[a-zA-Z_][a-zA-Z0-9_]* {
                  yylval.str = strdup(yytext);
                  return IDENTIFIER;
               }

"="            { return '='; }
"+"            { return '+'; }
"-"            { return '-'; }
[ \t\n]+       { /* ignore whitespace */ }

.              { printf("Unknown character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
