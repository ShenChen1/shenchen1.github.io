#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *__libc_malloc(size_t size);
void *__libc_calloc(size_t nmemb, size_t size);
void *__libc_realloc(void *ptr, size_t size);
void __libc_free(void *ptr);

void *malloc(size_t size)
{
	void *ptr;
	ptr = __libc_malloc(size);
	printf("[%s](%zu) = %p\n", __func__, size, ptr);
	return ptr;
}

void *calloc(size_t nmemb, size_t size)
{
	void *ptr;
	ptr = __libc_calloc(nmemb, size);
	printf("[%s](%zu, %zu) = %p\n", __func__, nmemb, size, ptr);
	return ptr;
}

void *realloc(void *old_ptr, size_t size)
{
	void *ptr;
	ptr = __libc_realloc(old_ptr, size);
	printf("[%s](%p, %zu) = %p\n", __func__, old_ptr, size, ptr);
	return ptr;
}

void free(void *ptr)
{
	if (ptr == NULL)
		return;

	printf("[%s](%p)\n", __func__, ptr);
	__libc_free(ptr);
}


