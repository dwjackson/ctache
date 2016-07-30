## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.

## Copyright (c) 2016 David Jackson

TESTS = test_hash_table
bin_PROGRAMS = test_hash_table
test_hash_table_SOURCES = test_hash_table.c utest.c
test_hash_table_CFLAGS = -g -Wall -I$(top_builddir)/include/ctache
test_hash_table_LDADD = $(top_builddir)/lib/libctache.la
