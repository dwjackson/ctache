/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include "linked_list.h"

enum ctache_token_type {
    CTACHE_TOK_STRING,
    CTACHE_TOK_SECTION_TAG_START,
    CTACHE_TOK_CLOSE_TAG_START,
    CTACHE_TOK_VALUE_TAG_START,
    CTACHE_TOK_TAG_END
};

struct ctache_token {
    char *value;
    enum ctache_token_type tok_type;
};

struct linked_list
*ctache_lex(const char *str, size_t str_len);

#endif /* LEXER_H */
