#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int func()
{
    char *p = malloc(10);
    
    strcpy(p, "func");

    printf("p:%s\n", p);

    free(p);

    return 0;
}
