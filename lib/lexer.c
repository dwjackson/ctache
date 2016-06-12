/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "lexer.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRVAL_INITIAL_BUFSIZE 10

char *ctache_token_names[] = {
    /* Terminals */
    "CTACHE_TOK_SECTION_TAG_START",
    "CTACHE_TOK_CLOSE_TAG_START",
    "CTACHE_TOK_VALUE_TAG_START",
    "CTACHE_TOK_TAG_END",
    "CTACHE_TOK_STRING",
    "CTACHE_TOK_EOI",

    /* Non-Terminals */
    "CTACHE_TOK_TEMPLATE",
    "CTACHE_TOK_TEXT",
    "CTACHE_TOK_TAG",
    "CTACHE_TOK_TAG_START"
};

static void
add_char_to_strval(char **strval_ptr,
                   size_t *strval_bufsize_ptr,
                   size_t *strval_len_ptr,
                   int ch)
{
    if (*strval_len_ptr + 1 < *strval_bufsize_ptr - 1) {
        (*strval_ptr)[*strval_len_ptr] = ch;
        (*strval_len_ptr)++;
    } else {
        (*strval_bufsize_ptr) *= 2;
        char *curr_strval = strdup(*strval_ptr);
        if (curr_strval == NULL) {
            fprintf(stderr, "ctache_lex(): Out of memory\n");
            exit(EXIT_FAILURE);
        }
        *strval_ptr = realloc(*strval_ptr, *strval_bufsize_ptr);
        memset(*strval_ptr, 0, *strval_bufsize_ptr);
        strcpy(*strval_ptr, curr_strval);

        free(curr_strval);
        curr_strval = NULL;

        (*strval_ptr)[*strval_len_ptr] = ch;
        (*strval_len_ptr)++;
    }
}

static struct ctache_token
*token_create(enum ctache_token_type token_type, char *value)
{
    struct ctache_token *tok;
    tok = malloc(sizeof(*tok));
    if (tok == NULL) {
        fprintf(stderr, "token_create(): Out of memory\n");
        exit(EXIT_FAILURE);
    }
    tok->tok_type = token_type;
    tok->value = value;
    return tok;
}

struct linked_list
*ctache_lex(const char *str, size_t str_len)
{
    struct linked_list *tokens = linked_list_create();
    if (tokens == NULL) {
        fprintf(stderr, "ctache_lex(): Cannot create tokens list\n");
        return NULL;
    }

    size_t strval_len = 0;
    size_t strval_bufsize = STRVAL_INITIAL_BUFSIZE;
    char *strval = malloc(strval_bufsize);
    memset(strval, 0, strval_bufsize);

    struct ctache_token *tok = NULL;
    int ch, i;
    for (i = 0; i < str_len; i++) {
        ch = str[i];
        switch (ch) {
        case '{':
            if (i + 3 < str_len) {
                if (str[i + 1] == '{' && str[i + 2] == '#') {
                    if (strval_len > 0) {
                        tok = token_create(CTACHE_TOK_STRING, strdup(strval));
                        strval_len = 0;
                        memset(strval, 0, strval_bufsize);
                        linked_list_append(tokens, tok);
                    }
                    tok = token_create(CTACHE_TOK_SECTION_TAG_START, NULL);
                    linked_list_append(tokens, tok);
                    i += 2;
                } else if (str[i + 1] == '{' && str[i + 2] == '/') {
                    if (strval_len > 0) {
                        tok = token_create(CTACHE_TOK_STRING, strdup(strval));
                        strval_len = 0;
                        linked_list_append(tokens, tok);
                        memset(strval, 0, strval_bufsize);
                    }
                    tok = token_create(CTACHE_TOK_CLOSE_TAG_START, NULL);
                    linked_list_append(tokens, tok);
                    i += 2;
                } else if (str[i + 1] == '{') {
                    if (strval_len > 0) {
                        tok = token_create(CTACHE_TOK_STRING, strdup(strval));
                        strval_len = 0;
                        linked_list_append(tokens, tok);
                        memset(strval, 0, strval_bufsize);
                    }
                    tok = token_create(CTACHE_TOK_VALUE_TAG_START, NULL);
                    linked_list_append(tokens, tok);
                    i += 1;
                } else {
                    add_char_to_strval(&strval,
                                       &strval_bufsize,
                                       &strval_len,
                                       ch);
                }
            } else {
                add_char_to_strval(&strval, &strval_bufsize, &strval_len, ch);
            }
            break;
        case '}':
            if (i + 1 < str_len && str[i + 1] == '}') {
                if (strval_len > 0) {
                    tok = token_create(CTACHE_TOK_STRING, strdup(strval));
                    strval_len = 0;
                    linked_list_append(tokens, tok);
                    memset(strval, 0, strval_bufsize);
                }
                tok = token_create(CTACHE_TOK_TAG_END, NULL);
                linked_list_append(tokens, tok);
                i += 1;
            } else {
                add_char_to_strval(&strval, &strval_bufsize, &strval_len, ch);
            }
            break;
        default:
            add_char_to_strval(&strval, &strval_bufsize, &strval_len, ch);
            break;
        }
    }
    if (strval_len > 0) {
        tok = token_create(CTACHE_TOK_STRING, strdup(strval));
        linked_list_append(tokens, tok);
    }
    free(strval);
    strval = NULL;

    /* Add a final EOI token */
    tok = token_create(CTACHE_TOK_EOI, NULL);
    linked_list_append(tokens, tok);

    return tokens;
}

#undef STRVAL_INITIAL_BUFSIZE
