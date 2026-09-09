/*
 * Run-time interposition of malloc and free based 
 * on the dynamic linker's (ld-linux.so) LD_PRELOAD mechanism
 * 
 * Example (Assume a.out calls malloc and free):
 *   linux> gcc -O2 -Wall -o mymalloc.so -shared mymalloc.c
 *
 *   tcsh> setenv LD_PRELOAD "/usr/lib/libdl.so ./mymalloc.so"
 *   tcsh> ./a.out
 *   tcsh> unsetenv LD_PRELOAD
 * 
 *   ...or 
 * 
 *   bash> (LD_PRELOAD="/usr/lib/libdl.so ./mymalloc.so" ./a.out)	
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

void *malloc(size_t size)
{
	static void *(*mallocp)(size_t size);
	char *error;
	void *ptr;

	/* get address of libc malloc */
	if (!mallocp) {
		mallocp = dlsym(RTLD_NEXT, "malloc");
		if ((error = dlerror()) != NULL) {
			fputs(error, stderr);
			exit(1);
		}
	}
	ptr = mallocp(size);
	printf("[%s](%zu) = %p\n", __func__, size, ptr);
	return ptr;
}

void *calloc(size_t nmemb, size_t size)
{
	static void *(*callocp)(size_t nmemb, size_t size);
	char *error;
	void *ptr;

	/* get address of libc malloc */
	if (!callocp) {
		callocp = dlsym(RTLD_NEXT, "calloc");
		if ((error = dlerror()) != NULL) {
			fputs(error, stderr);
			exit(1);
		}
	}
	ptr = callocp(nmemb, size);
	printf("[%s](%zu, %zu) = %p\n", __func__, nmemb, size, ptr);
	return ptr;
}

void *realloc(void *old_ptr, size_t size)
{
	static void *(*reallocp)(void *ptr, size_t size);
	char *error;
	void *ptr;

	/* get address of libc malloc */
	if (!reallocp) {
		reallocp = dlsym(RTLD_NEXT, "malloc");
		if ((error = dlerror()) != NULL) {
			fputs(error, stderr);
			exit(1);
		}
	}
	ptr = reallocp(old_ptr, size);
	printf("[%s](%p, %zu) = %p\n", __func__, old_ptr, size, ptr);
	return ptr;
}

void free(void *ptr)
{
	static void (*freep)(void *);
	char *error;

	if (ptr == NULL)
		return;

	/* get address of libc free */
	if (!freep) {
		freep = dlsym(RTLD_NEXT, "free");
		if ((error = dlerror()) != NULL) {
			fputs(error, stderr);
			exit(1);
		}
	}
	printf("[%s](%p)\n", __func__, ptr);
	freep(ptr);
}


