CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main -lm

run:
	./main 3 100000000

clean:
	rm -f main
