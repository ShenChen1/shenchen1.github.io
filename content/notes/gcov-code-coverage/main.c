#include <stdio.h>
#include <stdlib.h>

extern int func1(int n);
extern int func2(int n);

int main(int argc, char *argv[])
{
	int i = 0;
	int r = 0;
	if (argc == 2) {
		i = atoi(argv[1]);
		r = func1(i);
	} else {
		i = 10;
		r = func2(i);
	}

	printf("arg is %d\n", i);
	printf("result:%d\n", r);
}
