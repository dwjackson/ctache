# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(CTACHE_MAIN_HEADER ctache.h)
set(CTACHE_HEADERS ctache/ctache_array.h ctache/ctache_data.h
                   ctache/hash_table.h ctache/lexer.h ctache/linked_list.h
                   ctache/parser.h ctache/render.h)

install(FILES ${CTACHE_MAIN_HEADER} DESTINATION include)

install(FILES ${CTACHE_HEADERS} DESTINATION include/ctache)
