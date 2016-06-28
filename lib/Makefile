# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

prefix = /usr/local
CFLAGS = -Wall -O2 -fPIC
INCLUDE_FLAGS = -I../include -I../include/ctache

STATIC_LIBRARY = libctache.a
SHARED_LIBRARY = libctache.so
LIBRARY_SOURCES = ctache_array.c ctache_data.c hash_table.c lexer.c \
				  linked_list.c parser.c render.c escaping.c
LIBRARY_OBJECTS = ctache_array.o ctache_data.o hash_table.o lexer.o \
				  linked_list.o parser.o render.o escaping.o

all: $(STATIC_LIBRARY) $(SHARED_LIBRARY)

$(STATIC_LIBRARY): $(LIBRARY_OBJECTS)
	ar rcs $(STATIC_LIBRARY) $(LIBRARY_OBJECTS)

$(SHARED_LIBRARY): $(LIBRARY_OBJECTS)
	cc -shared -o $(SHARED_LIBRARY) $(LIBRARY_OBJECTS)

$(LIBRARY_OBJECTS): $(LIBRARY_SOURCES)
	cc $(CFLAGS) $(INCLUDE_FLAGS) -c $(LIBRARY_SOURCES)

clean:
	rm -f *.o
	rm -f $(STATIC_LIBRARY)
	rm -f $(SHARED_LIBRARY)

install: $(SHARED_LIBRARY) $(STATIC_LIBRARY)
	install $(SHARED_LIBRARY) $(prefix)/lib
	install $(STATIC_LIBRARY) $(prefix)/lib

.PHONY: all clean install
