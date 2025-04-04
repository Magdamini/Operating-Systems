CC = gcc
CFLAGS = -Wall

all: life
	
life: grid.c life.c
	$(CC) $(CFLAGS) grid.c life.c -o life -lncursesw  -lpthread -lm

test:
	./life 10
	
clean:
	rm -f life
