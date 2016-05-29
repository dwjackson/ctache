# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

BASE_NAME = ctache
CFLAGS = -Wall -g

EXE_NAME = $(BASE_NAME)
OBJ_FILES = main.o
SOURCE_FILES = main.c

LIB_NAME = lib$(BASE_NAME).so
LIB_OBJ_FILES = linked_list.o lexer.o
LIB_SOURCE_FILES = linked_list.c lexer.c

all: $(EXE_NAME) $(LIB_NAME)

$(EXE_NAME): $(OBJ_FILES) $(LIB_NAME)
	cc $(CFLAGS) -L. -o $(EXE_NAME) $(OBJ_FILES) -l$(BASE_NAME)

$(OBJ_FILES): $(SOURCE_FILES)
	cc $(CFLAGS) -c $(SOURCE_FILES)

$(LIB_NAME): $(LIB_OBJ_FILES)
	cc $(CFLAGS) -shared -o $(LIB_NAME) $(LIB_OBJ_FILES)

$(LIB_OBJ_FILES): $(LIB_SOURCE_FILES)
	cc $(CFLAGS) -c -fpic $(LIB_SOURCE_FILES)

clean:
	rm -f $(EXE_NAME)
	rm -f $(LIB_NAME)
	rm -f *.o

.PHONY: all clean
