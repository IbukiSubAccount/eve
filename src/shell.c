#include "include/shell.h"
#include <string.h>
#include <stdio.h>

char* get_shell_str(const char* printstr)
{
    char* str;
    int c;
    int i;
    int size = 10;

    str = malloc(size * sizeof(char));

    printf("%s", printstr);

    for(i = 0; (c = getchar()) != '\n' && c != EOF; ++i)
    {
        if(i == size)
        {
            size = 2*size;
            str = realloc(str, size*sizeof(char));

            if(str == NULL)
            {
                printf("ERROR: Cannot realloc string\n");
                exit(-1);
            }
        }
        str[i] = c;
    }

    if(i == size)
    {
        str = realloc(str, (size+1)*sizeof(char));

        if(str == NULL)
        {
            printf("ERROR: Cannot realloc string\n");
            exit(-1);
        }
    }

    return str;
}