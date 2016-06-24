/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESCAPING_H
#define ESCAPING_H

struct escape_char {
    char unescaped;
    char *escaped;
    size_t escaped_length;
};

char
*escape_html(const char *str);

#endif /* ESCAPING_H */
