## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.

## Copyright (c) 2016 David Jackson
## Modified work Copyright 2017 Daniel Araujo <contact@daniel-araujo.pt>

AM_CFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/include/ctache

lib_LTLIBRARIES = libctache.la
libctache_la_SOURCES = ctache_array.c ctache_array.h ctache_data.c \
					   hash_table.c hash_table.h lexer.c lexer.h \
					   linked_list.c linked_list.h parser.c parser.h render.c \
					   escaping.c ctache_string.c ctache_string.h
