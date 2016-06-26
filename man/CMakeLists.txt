# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(CTACHE_MAN_PAGE ctache.1)
set(CTACHE_RENDER_MAN_PAGE ctache_render.3)
set(CTACHE_DATA_MAN_PAGE ctache_render.3)

install(FILES ${CTACHE_MAN_PAGE} DESTINATION share/man/man1)
install(FILES ${CTACHE_RENDER_MAN_PAGE} DESTINATION share/man/man3)
install(FILES ${CTACHE_DATA_MAN_PAGE} DESTINATION share/man/man3)
