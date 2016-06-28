# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

CTACHE_MAN_PAGE = ctache.1
CTACHE_RENDER_MAN_PAGE = ctache_render.3
CTACHE_DATA_MAN_PAGE = ctache_data.3

MAN_DIR = $(prefix)/share/man

all:

install:
	install $(CTACHE_MAN_PAGE) $(MAN_DIR)/man1/$(CTACHE_MAN_PAGE)
	install $(CTACHE_RENDER_MAN_PAGE) $(MAN_DIR)/man3/$(CTACHE_RENDER_MAN_PAGE)
	install $(CTACHE_DATA_MAN_PAGE) $(MAN_DIR)/man3/$(CTACHE_DATA_MAN_PAGE)

uninstall:
	rm $(MAN_DIR)/man1/$(CTACHE_MAN_PAGE)
	rm $(MAN_DIR)/man3/$(CTACHE_RENDER_MAN_PAGE)
	rm $(MAN_DIR)/man3/$(CTACHE_DATA_MAN_PAGE)

.PHONY: all install uninstall
