# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

PROJECT_NAME = ctache
VERSION = 0.1.0

all: libraries executable

executable: libraries
	cd src && $(MAKE)

libraries:
	cd lib && $(MAKE)

clean:
	cd lib && $(MAKE) clean
	cd src && $(MAKE) clean

install:
	cd lib && $(MAKE) install
	cd src && $(MAKE) install

.PHONY: all clean install
