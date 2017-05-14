CC = gcc
CFLAGS = -g -std=c99 -Wno-unused-result -pipe -O3

all:  main test

main: main.o lexer.o tree.o transform.o
	$(CC) $^ -o $@

test: test.o lexer.o tree.o transform.o
	$(CC) $^ -o $@	

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f main test *.o db/*.o