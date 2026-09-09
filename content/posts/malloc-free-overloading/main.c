#include <stdio.h>
#include <stdlib.h>

int func();

int main()
{
    char *p = malloc(10);
    
    *p = 1;

    free(p);

    //*p = 2;

    func();

}
