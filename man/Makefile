# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

prefix = /usr/local
CTACHE_MAN_PAGE = ctache.1
CTACHE_RENDER_MAN_PAGE = ctache_render.3
CTACHE_DATA_MAN_PAGE = ctache_data.3

all:

install:
	install $(CTACHE_MAN_PAGE) $(prefix)/share/man/man1
	install $(CTACHE_RENDER_MAN_PAGE) $(prefix)/share/man/man3
	install $(CTACHE_DATA_MAN_PAGE) $(prefix)/share/man/man3

.PHONY: all install
