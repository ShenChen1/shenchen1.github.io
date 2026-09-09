#include <stdio.h>                                                                                  
#include <unistd.h>
#include <setjmp.h>

jmp_buf jmpbuf_th0;
jmp_buf jmpbuf_th1;

static int cnt1 = 0;
static int cnt0 = 0;

static void thread_0()
{
	printf("%s %d\n\n", __FUNCTION__, cnt0);
	sleep(1);
	longjmp(jmpbuf_th0, cnt0++);
}

static void thread_1()
{
	printf("%s %d\n\n", __FUNCTION__, cnt1);
	sleep(1);
	longjmp(jmpbuf_th1, cnt1++);
}


int main()
{
	int rc0, rc1 = 0;

entry_thread_0:
	rc0 = setjmp(jmpbuf_th0);
	printf("rc0 = %d\n", rc0);
	if (rc0 != 0)
		thread_1();
entry_thread_1:
	rc1 = setjmp(jmpbuf_th1);
	printf("rc1 = %d\n", rc1);
	thread_0();

	return 0;
}
