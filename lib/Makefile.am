# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

AM_CFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/include/ctache

lib_LTLIBRARIES = libctache.la
lib_LIBRARIES = libctache.a
libctache_la_SOURCES = ctache_array.c ctache_data.c hash_table.c lexer.c \
					   linked_list.c parser.c render.c escaping.c
libctache_a_SOURCES = libctache_la_sources
