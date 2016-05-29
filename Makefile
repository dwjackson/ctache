# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

BASE_NAME = ctache
CFLAGS = -Wall -g
PREFIX=/usr/local
CC = cc

# Executable
EXE_NAME = $(BASE_NAME)
OBJ_FILES = main.o
SOURCE_FILES = main.c

# Library
LIB_NAME = lib$(BASE_NAME).so
LIB_OBJ_FILES = linked_list.o lexer.o hash_table.o ctache_data.o
LIB_SOURCE_FILES = linked_list.c lexer.c hash_table.c ctache_data.c

all: $(EXE_NAME) $(LIB_NAME)

$(EXE_NAME): $(OBJ_FILES) $(LIB_NAME)
	$(CC) $(CFLAGS) -L. -o $(EXE_NAME) $(OBJ_FILES) -l$(BASE_NAME)

$(OBJ_FILES): $(SOURCE_FILES)
	$(CC) $(CFLAGS) -c $(SOURCE_FILES)

$(LIB_NAME): $(LIB_OBJ_FILES)
	$(CC) $(CFLAGS) -shared -o $(LIB_NAME) $(LIB_OBJ_FILES)

$(LIB_OBJ_FILES): $(LIB_SOURCE_FILES)
	$(CC) $(CFLAGS) -c -fpic $(LIB_SOURCE_FILES)

install: $(EXE_NAME) $(LIB_NAME)
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/lib
	install $(EXE_NAME) $(PREFIX)/bin/$(EXE_NAME)
	install $(LIB_NAME) $(PREFIX)/lib/$(LIB_NAME)

uninstall:
	rm $(PREFIX)/bin/$(EXE_NAME)
	rm $(PREFIX)/lib/$(LIB_NAME)

clean:
	rm -f $(EXE_NAME)
	rm -f $(LIB_NAME)
	rm -f *.o

.PHONY: all clean uninstall
