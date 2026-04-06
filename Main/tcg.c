#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100

static char code[MAX_LINES][100];
static int lineCount = 0;

char* generateTargetCode(char* input)
{
    // Parse input into lines
    lineCount = 0;
    char* token = strtok(input, "\n");
    while (token && lineCount < MAX_LINES) {
        strcpy(code[lineCount++], token);
        token = strtok(NULL, "\n");
    }

    static char targetBuffer[10000];
    int pos = 0;

    for(int i=0;i<lineCount;i++)
    {
        char var[20];
        char expr[100];

        /* handle declarations */
        if(sscanf(code[i],"int %[^=]=%[^\n]",var,expr)==2)
        {
            pos += sprintf(targetBuffer + pos, "int %s = %s;\n", var, expr);
        }

        /* handle assignments */
        else if(sscanf(code[i],"%[^=]=%[^\n]",var,expr)==2)
        {
            pos += sprintf(targetBuffer + pos, "%s = %s;\n", var, expr);
        }

        /* handle print */
        else if(sscanf(code[i],"PRINT %[^\n]",expr)==1)
        {
            pos += sprintf(targetBuffer + pos, "printf(\"%%d\\n\", %s);\n", expr);
        }
    }

    return strdup(targetBuffer);
}
