CXX = gcc
CFLAGS = -Wall

all: reverse_by_bytes reverse_by_blocks

reverse_by_bytes: 
	gcc $(CFLAGS) reverse_by_bytes.c -o reverse_by_bytes

reverse_by_blocks:
	gcc $(CFLAGS) reverse_by_blocks.c -o reverse_by_blocks

.PHONY: clean
clean:
	rm -f reverse_by_blocks
	rm -f reverse_by_bytes
	rm -f reversed*
	rm -f double_reversed*

test:
	./reverse_by_bytes test.txt reversed_test_bytes.txt
	./reverse_by_blocks test.txt reversed_test_blocks.txt

test_double_reverse:
	./reverse_by_bytes test.txt reversed_bytes.txt
	./reverse_by_bytes reversed_bytes.txt double_reversed_bytes.txt
	diff -s test.txt double_reversed_bytes.txt

	./reverse_by_blocks test.txt reversed_blocks.txt
	./reverse_by_blocks reversed_blocks.txt double_reversed_blocks.txt
	diff -s test.txt double_reversed_blocks.txt
	