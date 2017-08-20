## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.

## Copyright (c) 2016 David Jackson

AM_CFLAGS = -Wall -Wextra -I$(top_srcdir)/include -I$(top_srcdir)/lib \
	    -I$(top_srcdir)

bin_PROGRAMS = ctache
ctache_SOURCES = ctache.c yaml_data.c yaml_data.h json.c json.h strbuf.c \
		 strbuf.h json_data.c json_data.h
ctache_LDADD = $(top_srcdir)/lib/libctache.la
