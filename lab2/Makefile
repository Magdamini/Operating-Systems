CFLAGS = -Wall -O0

all: main_static main_shared main_dynamic

main_static: libcollatz.a
	gcc $(CFLAGS) main.c libcollatz.a -o main_static
	

main_shared: libcollatz.so
	gcc $(CFLAGS) main.c -Wl,-rpath=. -L. -lcollatz -o main_shared
	

main_dynamic: libcollatz.so
	gcc $(CFLAGS) main.c -o main_dynamic -ldl -DDYNAMIC


libcollatz.a:
	gcc -c collatz.c
	ar rcs libcollatz.a collatz.o

libcollatz.so:
	gcc -fPIC -c collatz.c
	gcc -shared collatz.o -o libcollatz.so

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f main_static
	rm -f main_shared
	rm -f main_dynamic

test:
	./main_static
	./main_shared
	./main_dynamic
	
	
	
