#include <stdio.h>

static int _func1(int n)
{
	return n * n;
}

static int _func2(int n)
{
	return 2 * n;
}

int func1(int n)
{
	return _func1(n) + _func2(n);
}

int func2(int n)
{
	return _func1(n) - _func2(n);
}
