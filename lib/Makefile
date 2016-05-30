# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

BASE_NAME = ctache
CFLAGS = -Wall -g -I../include/ctache
PREFIX=/usr/local
CC = cc

# Library
LIB_NAME = lib$(BASE_NAME).so
LIB_OBJ_FILES = linked_list.o lexer.o hash_table.o ctache_data.o ctache_array.o
LIB_SRC_FILES = linked_list.c lexer.c hash_table.c ctache_data.c ctache_array.c

all: $(LIB_NAME)

$(LIB_NAME): $(LIB_OBJ_FILES)
	$(CC) $(CFLAGS) -shared -o $(LIB_NAME) $(LIB_OBJ_FILES)

$(LIB_OBJ_FILES): $(LIB_SRC_FILES)
	$(CC) $(CFLAGS) -c -fpic $(LIB_SRC_FILES)

clean:
	rm -f $(LIB_NAME)
	rm -f *.o

.PHONY: all clean
