#include <stdio.h>
#include <string.h>

#define MAX_LINES 100

char code[MAX_LINES][100];
int lineCount = 0;

int main()
{
    printf("Enter optimized code (type END to stop):\n");

    while (1)
    {
        fgets(code[lineCount],100,stdin);
        code[lineCount][strcspn(code[lineCount],"\n")] = 0;

        if(strcmp(code[lineCount],"END")==0)
            break;

        lineCount++;
    }

    printf("\nTarget Code:\n");

    for(int i=0;i<lineCount;i++)
    {
        char var[20];
        int value;

        /* handle assignment x = 4 */
        if(sscanf(code[i],"%s = %d",var,&value)==2)
        {
            printf("MOV R1, %d\n",value);
            printf("MOV %s, R1\n",var);
        }

        /* ignore declarations */
        else if(strncmp(code[i],"declare",7)==0)
        {
            continue;
        }
    }

    return 0;
}
