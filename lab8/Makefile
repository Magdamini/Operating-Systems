CFLAGS = -Wall -std=gnu99


all: clean main

main:
	gcc $(CFLAGS) main.c -o main -lpthread -lrt

test:
	./main 5 2

clean:
	rm -f main
