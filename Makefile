# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

PROJECT_NAME = ctache
VERSION = 0.1.0
prefix = /usr/local
export prefix

all: libraries executable

executable: libraries
	cd src && $(MAKE)

libraries:
	cd lib && $(MAKE)

clean install uninstall:
	cd lib && $(MAKE) $@
	cd src && $(MAKE) $@

.PHONY: all clean install
