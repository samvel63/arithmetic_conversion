CC = gcc
CFLAGS = -g -std=c99 -Wno-unused-result -pipe -O3

all:  main

main: main.o
	$(CC) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f main *.o db/*.o