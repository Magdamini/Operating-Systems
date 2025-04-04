CFLAGS = -Wall -std=gnu99

all: handle_signal

handle_signal:
	gcc $(CFLAGS) handle_signal.c -o handle_signal

clean:
	rm -f handle_signal


test:
	./handle_signal ignore
	./handle_signal handler
	./handle_signal mask
	./handle_signal none
	
	
	
