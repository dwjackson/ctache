/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#ifndef CTACHE_ESCAPING_H
#define CTACHE_ESCAPING_H

struct escape_char {
    char unescaped;
    char *escaped;
    size_t escaped_length;
};

enum escaping_type {
    ESCAPE_HTML,
    ESCAPE_TEX,
    ESCAPE_NONE
};

char
*ctache_escape_text(const char *str, enum escaping_type);

#endif /* CTACHE_ESCAPING_H */
