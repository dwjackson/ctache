# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(EXE_SOURCES main.c yaml_data.c)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(${EXE_NAME} ${EXE_SOURCES})
target_link_libraries(${EXE_NAME} ${STATIC_LIB_NAME} yaml) 
set_target_properties(${EXE_NAME} PROPERTIES OUTPUT_NAME ctache)

install(TARGETS ${EXE_NAME}
        RUNTIME DESTINATION bin)
