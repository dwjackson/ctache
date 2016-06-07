set(LIBRARY_SOURCES ctache_array.c ctache_data.c hash_table.c lexer.c
                    linked_list.c)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/ctache)
add_library(${LIB_NAME} SHARED ${LIBRARY_SOURCES})
