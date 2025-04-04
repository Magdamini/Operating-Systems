CFLAGS = -Wall -std=gnu99

TIME := /usr/bin/time

all: integral

integral:
	gcc $(CFLAGS) integral.c -o integral -lm

clean:
	rm -f integral


test:
	./integral 0.01 10

time:
	./integral 0.0000000005 20
	./integral 0.0000000005 1
	./integral 0.00000001 20
	./integral 0.00000001 1

	
	
	
