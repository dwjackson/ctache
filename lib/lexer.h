/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#ifndef CTACHE_LEXER_H
#define CTACHE_LEXER_H

#include <stdlib.h>
#include "linked_list.h"

enum ctache_token_type {
    /* Terminals */
    CTACHE_TOK_SECTION_TAG_START,
    CTACHE_TOK_CLOSE_TAG_START,
    CTACHE_TOK_VALUE_TAG_START,
    CTACHE_TOK_UNESC_VALUE_TAG_START,
    CTACHE_TOK_PARTIAL_TAG,
    CTACHE_TOK_TAG_END,
    CTACHE_TOK_STRING,
    CTACHE_TOK_EOI,

    /* Non-Terminals */
    CTACHE_TOK_TEMPLATE,
    CTACHE_TOK_TEXT,
    CTACHE_TOK_TAG,
    CTACHE_TOK_TAG_START
};
#define CTACHE_NUM_TERMINALS (CTACHE_TOK_EOI + 1)
#define CTACHE_NUM_NONTERMINALS (CTACHE_TOK_TAG_START - CTACHE_TOK_TEMPLATE + 1)

struct ctache_token {
    char *value; /* If not-NULL, the string content of the token */
    enum ctache_token_type tok_type; /* The token's type */
    int line; /* Line number in the original file where this token occurred */
    int character; /* Character index in the line where this token occurred */
};

struct linked_list
*ctache_lex(const char *str,
            size_t str_len,
            const char *delim_begin,
            const char *delim_end);

#endif /* CTACHE_LEXER_H */
