/* 
 * Link-time interposition of malloc and free using the static
 * linker's (ld) "--wrap symbol" flag.
 * 
 * Compile the executable using "-Wl,--wrap,malloc -Wl,--wrap,free".
 * This tells the linker to resolve references to malloc as
 * __wrap_malloc, free as __wrap_free, __real_malloc as malloc, and
 * __real_free as free.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *__real_malloc(size_t size);
void *__real_calloc(size_t nmemb, size_t size);
void *__real_realloc(void *ptr, size_t size);
void __real_free(void *ptr);

void *__wrap_malloc(size_t size)
{
	void *ptr;
	ptr = __real_malloc(size);
	printf("[%s](%zu) = %p\n", __func__, size, ptr);
	return ptr;
}

void *__wrap_calloc(size_t nmemb, size_t size)
{
	void *ptr;
	ptr = __real_calloc(nmemb, size);
	printf("[%s](%zu, %zu) = %p\n", __func__, nmemb, size, ptr);
	return ptr;
}

void *__wrap_realloc(void *old_ptr, size_t size)
{
	void *ptr;
	ptr = __real_realloc(old_ptr, size);
	printf("[%s](%p, %zu) = %p\n", __func__, old_ptr, size, ptr);
	return ptr;
}

void __wrap_free(void *ptr)
{
	if (ptr == NULL)
		return;
	
	printf("[%s](%p)\n", __func__, ptr);
	__real_free(ptr);
}


