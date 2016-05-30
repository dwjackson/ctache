# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

BASE_NAME = ctache
CFLAGS = -Wall -g -I../include
PREFIX=/usr/local
CC = cc

# Executable
EXE_NAME = $(BASE_NAME)
OBJ_FILES = main.o
SRC_FILES = main.c
LIB_NAME=../lib/lib$(BASE_NAME).so

all: $(EXE_NAME)

$(EXE_NAME): $(OBJ_FILES) $(LIB_NAME)
	$(CC) $(CFLAGS) -L../lib -o $(EXE_NAME) $(OBJ_FILES) -l$(BASE_NAME)

$(OBJ_FILES): $(SRC_FILES)
	$(CC) $(CFLAGS) -c $(SRC_FILES)

clean:
	rm -f $(EXE_NAME)
	rm -f *.o

.PHONY: all clean
