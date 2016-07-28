CFLAGS = -g -Wall -I../include/ctache -L../lib

all: test_hash_table

test_hash_table: test_hash_table.o
	cc $(CFLAGS) -o test_hash_table test_hash_table.c

test_hash_table.o: test_hash_table.c
	cc $(CFLAGS) -c test_hash_table.c

clean:
	rm -f test_hash_table
	rm -f *.o

.PHONY: all clean
