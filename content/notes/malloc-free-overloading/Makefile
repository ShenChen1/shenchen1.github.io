CC := gcc

all:malloc-glibc malloc-dl malloc-wrap

clean:
	rm -rf *.so *.o malloc-*

libtest.so:test.c
	$(CC) -O2 -g -fPIC -Wall -o $@ -shared $^

mymalloc1.so:mymalloc1.c
	$(CC) -O2 -g -fPIC -Wall -o $@ -shared $^

mymalloc2.so:mymalloc2.c
	$(CC) -O2 -g -fPIC -Wall -o $@ -shared $^

mymalloc3.o:mymalloc3.c
	$(CC) -c -O2 -g -Wall -o $@ $^

malloc-glibc:main.c libtest.so mymalloc1.so
	$(CC) -o $@ main.c libtest.so

malloc-dl:main.c libtest.so mymalloc2.so
	$(CC) -o $@ $^ -ldl

malloc-wrap:main.c libtest.so mymalloc3.o
	$(CC) -o $@ $^ -Wl,--wrap,malloc -Wl,--wrap,free -Wl,--wrap,realloc -Wl,--wrap,calloc 

