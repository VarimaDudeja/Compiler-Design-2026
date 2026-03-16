#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100

char code[MAX_LINES][100];
char optimized[MAX_LINES][100];
int lineCount = 0;

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

int main()
{
    printf("Enter intermediate code (type END to stop):\n");

    while(1)
    {
        fgets(code[lineCount],100,stdin);
        code[lineCount][strcspn(code[lineCount],"\n")] = 0;

        if(strcmp(code[lineCount],"END")==0)
            break;

        lineCount++;
    }

    int optLine=0;

    for(int i=0;i<lineCount;i++)
    {
        char a[20],b[20];
        int val;

        /* case: t0 = 4 */
        if(sscanf(code[i],"%s = %d",a,&val)==2)
        {
            setTemp(a,val);
        }

        /* case: x = t0 */
        else if(sscanf(code[i],"%s = %s",a,b)==2)
        {
            int v=getTemp(b);
            if(v!=-1)
                sprintf(optimized[optLine++],"%s = %d",a,v);
            else
                strcpy(optimized[optLine++],code[i]);
        }

        else
            strcpy(optimized[optLine++],code[i]);
    }

    printf("\nOptimized Code:\n");

    for(int i=0;i<optLine;i++)
        printf("%s\n",optimized[i]);

    return 0;
}
