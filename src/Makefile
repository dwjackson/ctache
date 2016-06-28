# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

prefix = /usr/local
EXE_NAME = ctache
EXE_OBJECTS = main.o yaml_data.o
EXE_SOURCES = main.c yaml_data.c
INCLUDE_FLAGS = -I../include
CFLAGS = -Wall -O2 $(INCLUDE_FLAGS)

all: $(EXE_NAME)

$(EXE_NAME): $(EXE_OBJECTS)
	cc $(CFLAGS) -o $(EXE_NAME) -L../lib $(EXE_OBJECTS) -lctache -lyaml

$(EXE_OBJECTS): $(EXE_SOURCES)
	cc $(CFLAGS) -c $(EXE_SOURCES)

clean:
	rm -f *.o
	rm -f $(EXE_NAME)

install: $(EXE_NAME)
	install $(EXE_NAME) $(prefix)/bin

.PHONY: all clean install
