#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("hello 1\n");
    printf("hello 2\n");
    printf("hello 3\n");
    printf("hello %s\n", argv[0]);
    printf("hello %s %s\n", argv[0]);
    return 0;
}
