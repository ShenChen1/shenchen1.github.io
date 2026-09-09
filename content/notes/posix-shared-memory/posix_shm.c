#include <assert.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int fd;
	fd = shm_open("test", (O_RDWR | O_CREAT), (S_IRUSR | S_IWUSR));
	assert(fd != -1);

	/* size the object.  We make it 4000 bytes long.  */
	assert(ftruncate(fd, 4000) != -1);

	struct stat st;
	assert(fstat(fd, &st) != -1);
	assert(st.st_size == 4000);

	/* mmap the test shmem object */
	void *shm_mem = mmap(NULL, st.st_size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
	/* write something */
	strcpy(shm_mem, "123456");

	if (argc > 1) {
		/* we delete the shmem object first */
		shm_unlink("test");
	}
	close(fd);

	if (argc > 1) {
		/* check shmem object deleted already */
		fd = shm_open("test", (O_RDWR), (S_IRUSR | S_IWUSR));
		assert(fd == -1);
	}

	/* create a shmem object which has the same name. */
	fd = shm_open("test", (O_RDWR | O_CREAT), (S_IRUSR | S_IWUSR));
	assert(fd != -1);

	/* size the object.  We make it 4000 bytes long.  */
	assert(ftruncate(fd, 4000) != -1);

	void *shm_mem1 = mmap(NULL, st.st_size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
	/* write something */
	strcpy(shm_mem1, "abcdef");

	printf("[%p]:%s\n", shm_mem, shm_mem);
	printf("[%p]:%s\n", shm_mem1, shm_mem1);

	if (argc > 1) {
		assert(strcmp(shm_mem, shm_mem1) != 0);
	} else {
		assert(strcmp(shm_mem, shm_mem1) == 0);
	}

	munmap(shm_mem, st.st_size);
	munmap(shm_mem1, st.st_size);

	close(fd);
}
