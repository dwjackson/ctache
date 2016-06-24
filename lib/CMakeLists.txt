# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(LIBRARY_SOURCES ctache_array.c ctache_data.c hash_table.c lexer.c
                    linked_list.c parser.c render.c escaping.c)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/ctache)
add_library(${SHARED_LIB_NAME} SHARED ${LIBRARY_SOURCES})
add_library(${STATIC_LIB_NAME} ${LIBRARY_SOURCES})
set_target_properties(${SHARED_LIB_NAME} PROPERTIES OUTPUT_NAME ctache)
set_target_properties(${STATIC_LIB_NAME} PROPERTIES OUTPUT_NAME ctache)

install(TARGETS ${SHARED_LIB_NAME} ${STATIC_LIB_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
