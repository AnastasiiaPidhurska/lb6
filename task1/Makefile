CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main -lm

run:
	./main 10

clean:
	rm -f main
