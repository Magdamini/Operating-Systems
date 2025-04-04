CFLAGS = -Wall -std=gnu99


all: santa_claus

santa_claus:
	gcc $(CFLAGS) santa_claus.c -o santa_claus -lpthread -lrt

test:
	./santa_claus

clean:
	rm -f santa_claus