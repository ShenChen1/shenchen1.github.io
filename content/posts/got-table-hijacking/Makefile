CC := gcc

all:test.out

clean:
	rm -rf *.out

test.out:test.c stub.c
	$(CC) -O2 -g -o  $@ $^ 
