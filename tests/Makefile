CFLAGS = -g -Wall -I../include/ctache -L../lib/.libs

all: test_hash_table

test_hash_table: test_hash_table.o utest.o
	cc $(CFLAGS) -o test_hash_table test_hash_table.o utest.o -lctache

test_hash_table.o: test_hash_table.c
	cc $(CFLAGS) -c test_hash_table.c

utest.o: utest.c
	cc $(CFLAGS) -c utest.c

clean:
	rm -f test_hash_table
	rm -f *.o

.PHONY: all clean
