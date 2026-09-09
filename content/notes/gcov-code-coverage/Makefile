CFLAGS := -Os -g -Wall -Werror -fprofile-arcs -ftest-coverage

all: clean lib app cov

clean:
	rm -rf *.so test
	rm -rf *.gcov *.gcda *.gcno
	rm -rf *.info result 

lib:
	gcc $(CFLAGS) -fPIC -shared test.c -o libtest.so 

app:
	gcc $(CFLAGS) main.c -o test -L. -ltest -Wl,-rpath=./
	./test 1
	./test 2

cov:
	gcov *.c
	lcov -d . -b . -c -o test.info
	genhtml -o result test.info
