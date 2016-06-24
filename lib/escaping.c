/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "escaping.h"

static char
*_escape(const char *str,
         struct escape_char escape_chars[],
         size_t num_escape_chars)
{
    char *escaped_str = NULL;

    size_t str_length = strlen(str);
    size_t escaped_length = 0;

    char ch;
    int i, j;
    struct escape_char ech;
    bool found;
    for (i = 0; i < str_length; i++) {
        ch = str[i];
        found = false;
        for (j = 0; j < num_escape_chars; j++) {
            ech = escape_chars[j];
            if (ch == ech.unescaped) {
                escaped_length += ech.escaped_length;
                break;
            }
        }
        if (!found) {
            escaped_length++;
        }
    }

    escaped_str = malloc(escaped_length + 1);
    memset(escaped_str, 0, escaped_length + 1);
    int escaped_index = 0;
    if (escaped_str != NULL) {
        for (i = 0; i < str_length; i++) {
            ch = str[i];
            found = false;
            for (j = 0; j < num_escape_chars; j++) {
                ech = escape_chars[j];
                if (ch == ech.unescaped) {
                    strcat(escaped_str, ech.escaped);
                    escaped_index += ech.escaped_length;
                    found = true;
                    break;
                }
            }
            if (!found) {
                escaped_str[escaped_index] = ch;
                escaped_index++;
            }
        }
    }
    
    return escaped_str;
}

/* Return a dynamically-allocated, HTML-escaped string */
char
*escape_html(const char *str)
{
    static struct escape_char escape_chars[] = {
        { '&', "&amp;", 5 },
        { '<', "&lt;",  4 },
        { '>', "&gt;",  4 }
    };
    static size_t num_escape_chars = 3;

    return _escape(str, escape_chars, num_escape_chars);
}

/* Return a dynamically-allocated, TeX-escaped string */
char
*escape_tex(const char *str)
{
    static struct escape_char escape_chars[] = {
        {  '{',  "\\{", 2 },
        {  '}',  "\\}", 2 },
        {  '&',  "\\&", 2 },
        {  '%',  "\\%", 2 },
        { '\\', "\\\\", 2 },
        {  '_',  "\\_", 2 },
        {  '#',  "\\#", 2 },
        {  '^',  "\\^", 2 },
        {  '~',  "\\~", 2 },
        {  '$',  "\\$", 2 }
    };
    size_t num_escape_chars;
    num_escape_chars = sizeof(escape_chars) / sizeof(struct escape_char); 

    return _escape(str, escape_chars, num_escape_chars);
}
