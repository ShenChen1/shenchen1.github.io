#include <stdio.h>

int myabs(int *a)
{
	if (*a < 0)
		return *a * -1;
	return *a;
}

int sum_abs(int *a, int n)
{
	int result = 0, i;

	for (i = 0; i < n; i++)
		result += myabs(&a[i]);
	return result;
}

int main(void)
{
	int result;
	int array[5] = {1,2,3,4,5};

	result = sum_abs(array, 1000*1000);
	printf("Result is %d\n", result);
	return 0;
}
