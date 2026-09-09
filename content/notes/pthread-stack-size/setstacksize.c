#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <limits.h>

static void *threadFunc(void *arg)
{
	printf("Hello fromt threadFunc\n");
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t t;
	pthread_attr_t thread_attr;
	int s = 0;
	size_t tmp_size = 0;

	s = pthread_attr_init(&thread_attr);
	assert(s == 0);

	s = pthread_attr_getstacksize(&thread_attr, &tmp_size);
	assert(s == 0);

	printf("default stack size of pthread is:%zu\n", tmp_size);

	s = pthread_attr_setstacksize(&thread_attr, PTHREAD_STACK_MIN + 0x1000);
	assert(s == 0);

	s = pthread_attr_getstacksize(&thread_attr, &tmp_size);
	assert(s == 0);

	printf("forced stack size of pthread is:%zu\n", tmp_size);

	s = pthread_create(&t, &thread_attr, threadFunc, NULL);
	assert(s == 0);

	getchar();

	printf("Main done()\n");
	return 0;
}
