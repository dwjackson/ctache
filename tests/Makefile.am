## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.

## Copyright (c) 2016-2020 David Jackson
## Modified work Copyright 2017 Daniel Araujo

TESTS = test_hash_table test_delimiters test_render test_boolean test_array \
	       test_string test_json test_number

check_PROGRAMS = test_hash_table test_delimiters test_render test_boolean \
	       test_array test_string test_json test_number

test_hash_table_SOURCES = test_hash_table.c
test_hash_table_CFLAGS = -g -Wall -I$(top_builddir)/include/ctache
test_hash_table_LDADD = $(top_builddir)/lib/libctache.la -lastrounit

test_delimiters_SOURCES = test_delimiters.c
test_delimiters_CFLAGS = -g -Wall -I$(top_builddir)/include/ctache
test_delimiters_LDADD = $(top_builddir)/lib/libctache.la -lastrounit

test_render_SOURCES = test_render.c
test_render_CFLAGS = -g -Wall -I$(top_builddir)/include/ctache
test_render_LDADD = $(top_builddir)/lib/libctache.la -lastrounit

test_boolean_SOURCES = test_boolean.c
test_boolean_CFLAGS = -g -Wall -I$(top_builddir)/include/ctache
test_boolean_LDADD = $(top_builddir)/lib/libctache.la -lastrounit

test_array_SOURCES = test_array.c
test_array_CFLAGS = -g -Wall -I$(top_builddir)/include/ctache
test_array_LDADD = $(top_builddir)/lib/libctache.la -lastrounit

test_string_SOURCES = test_string.c
test_string_CFLAGS = -g -Wall -I$(top_builddir)/include/ctache
test_string_LDADD = $(top_builddir)/lib/libctache.la -lastrounit

test_json_SOURCES = test_json.c $(top_builddir)/src/json.c \
		    $(top_builddir)/src/json.h $(top_builddir)/src/strbuf.c \
		    $(top_builddir)/src/strbuf.h
test_json_CFLAGS = -g -Wall -I$(top_builddir)/src
test_json_LDADD = $(top_builddir)/lib/libctache.la -lastrounit

test_number_SOURCES = test_number.c $(top_builddir)/src/json.c \
		    $(top_builddir)/src/json.h $(top_builddir)/src/strbuf.c \
		    $(top_builddir)/src/strbuf.h
test_number_CFLAGS = -g -Wall -I$(top_builddir)/include/ctache
test_number_LDADD = $(top_builddir)/lib/libctache.la -lastrounit
