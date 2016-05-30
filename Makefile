# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

BASE_NAME = ctache
CFLAGS = -Wall -g
PREFIX=/usr/local
CC = cc

EXE_NAME = $(BASE_NAME)
LIB_NAME = lib$(BASE_NAME).so

all: $(EXE_NAME) $(LIB_NAME)

$(EXE_NAME): $(LIB_NAME)
	cd src && $(MAKE)

$(LIB_NAME):
	cd lib && $(MAKE)

install: $(EXE_NAME) $(LIB_NAME)
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/lib
	install $(EXE_NAME) $(PREFIX)/bin/$(EXE_NAME)
	install $(LIB_NAME) $(PREFIX)/lib/$(LIB_NAME)

uninstall:
	rm $(PREFIX)/bin/$(EXE_NAME)
	rm $(PREFIX)/lib/$(LIB_NAME)

clean:
	cd src && $(MAKE) clean
	cd lib && $(MAKE) clean

.PHONY: all clean uninstall
