#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100

static char code[MAX_LINES][100];
static char optimized[MAX_LINES][100];
static int lineCount = 0;

/* store temporary variable values */
char tempNames[50][20];
int tempValues[50];
int tempCount = 0;

void setTemp(char *name, int val)
{
    strcpy(tempNames[tempCount], name);
    tempValues[tempCount] = val;
    tempCount++;
}

int getTemp(char *name)
{
    for(int i=0;i<tempCount;i++)
    {
        if(strcmp(tempNames[i],name)==0)
            return tempValues[i];
    }
    return -1;
}

char* optimizeCode(char* input)
{
    // Parse input into lines
    lineCount = 0;
    char* token = strtok(input, "\n");
    while (token && lineCount < MAX_LINES) {
        strcpy(code[lineCount++], token);
        token = strtok(NULL, "\n");
    }

    tempCount = 0;

    static char optBuffer[10000];
    int optPos = 0;

    for(int i=0;i<lineCount;i++)
    {
        char a[20],b[20];
        int val;

        /* handle declarations */
        if(strncmp(code[i],"int ",4)==0)
        {
            optPos += sprintf(optBuffer + optPos, "%s\n", code[i]);
        }

        /* case: t0 = 4 */
        else if(sscanf(code[i],"%s=%d",a,&val)==2)
        {
            setTemp(a,val);
        }

        /* case: x = t0 */
        else if(sscanf(code[i],"%s=%s",a,b)==2)
        {
            int v=getTemp(b);
            if(v!=-1)
                optPos += sprintf(optBuffer + optPos,"%s=%d\n",a,v);
            else
                optPos += sprintf(optBuffer + optPos,"%s\n",code[i]);
        }

        else
            optPos += sprintf(optBuffer + optPos,"%s\n",code[i]);
    }

    return strdup(optBuffer);
}
