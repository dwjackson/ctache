/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
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
    "CTACHE_TOK_UNESC_VALUE_TAG_START",
    "CTACHE_TOK_PARTIAL_TAG",
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
*token_create(enum ctache_token_type token_type, char *value, int row, int col)
{
    struct ctache_token *tok;
    tok = malloc(sizeof(*tok));
    if (tok == NULL) {
        fprintf(stderr, "token_create(): Out of memory\n");
        exit(EXIT_FAILURE);
    }
    tok->tok_type = token_type;
    tok->value = value;
    tok->line = row + 1;
    tok->character = col + 1;
    return tok;
}

struct linked_list
*ctache_lex(const char *str,
            size_t str_len,
            const char *delim_begin,
            const char *delim_end)
{
    struct linked_list *tokens = linked_list_create();
    if (tokens == NULL) {
        fprintf(stderr, "ctache_lex(): Cannot create tokens list\n");
        return NULL;
    }

    size_t delim_begin_len = strlen(delim_begin);
    size_t delim_end_len = strlen(delim_end);
    size_t strval_len = 0;
    size_t strval_bufsize = STRVAL_INITIAL_BUFSIZE;
    char *strval = malloc(strval_bufsize);
    memset(strval, 0, strval_bufsize);

    int row = 0;
    int col = 0;
    struct ctache_token *tok = NULL;
    int ch, i;
    for (i = 0; i < str_len; i++) {
        ch = str[i];
        if (ch == delim_begin[0]) {
            if (i + delim_begin_len + 1 < str_len) {
                if (strncmp(str + i, delim_begin, delim_begin_len) == 0
                    && str[i + delim_begin_len] == '#') {
                    if (strval_len > 0) {
                        tok = token_create(CTACHE_TOK_STRING,
                                           strdup(strval),
                                           row,
                                           col);
                        strval_len = 0;
                        memset(strval, 0, strval_bufsize);
                        linked_list_append(tokens, tok);
                    }
                    tok = token_create(CTACHE_TOK_SECTION_TAG_START,
                                       NULL,
                                       row,
                                       col);
                    linked_list_append(tokens, tok);
                    i += delim_begin_len;
                    col += delim_begin_len;
                } else if (strncmp(str + i, delim_begin, delim_begin_len) == 0
                           && str[i + delim_begin_len] == '/') {
                    if (strval_len > 0) {
                        tok = token_create(CTACHE_TOK_STRING,
                                           strdup(strval),
                                           row,
                                           col);
                        strval_len = 0;
                        linked_list_append(tokens, tok);
                        memset(strval, 0, strval_bufsize);
                    }
                    tok = token_create(CTACHE_TOK_CLOSE_TAG_START,
                                       NULL,
                                       row,
                                       col);
                    linked_list_append(tokens, tok);
                    i += delim_begin_len;
                    col += delim_begin_len;
                } else if (strncmp(str + i, delim_begin, delim_begin_len) == 0
                           && str[i + delim_begin_len] == '>') {
                    if (strval_len > 0) {
                        tok = token_create(CTACHE_TOK_STRING,
                                           strdup(strval),
                                           row,
                                           col);
                        strval_len = 0;
                        linked_list_append(tokens, tok);
                        memset(strval, 0, strval_bufsize);
                    }
                    tok = token_create(CTACHE_TOK_PARTIAL_TAG,
                                       NULL,
                                       row,
                                       col);
                    linked_list_append(tokens, tok);
                    i += delim_begin_len;
                } else if (strncmp(str + i, delim_begin, delim_begin_len) == 0
                           && (str[i + delim_begin_len] == '&'
                               || str[i + delim_begin_len] == delim_begin[0])) {
                    if (strval_len > 0) {
                        tok = token_create(CTACHE_TOK_STRING,
                                           strdup(strval),
                                           row,
                                           col);
                        strval_len = 0;
                        linked_list_append(tokens, tok);
                        memset(strval, 0, strval_bufsize);
                    }
                    tok = token_create(CTACHE_TOK_UNESC_VALUE_TAG_START,
                                       NULL,
                                       row,
                                       col);
                    linked_list_append(tokens, tok);
                    i += delim_begin_len;
                    col += delim_begin_len;
                } else if (strncmp(str + i, delim_begin, delim_begin_len) == 0) {
                    if (strval_len > 0) {
                        tok = token_create(CTACHE_TOK_STRING,
                                           strdup(strval),
                                           row,
                                           col);
                        strval_len = 0;
                        linked_list_append(tokens, tok);
                        memset(strval, 0, strval_bufsize);
                    }
                    tok = token_create(CTACHE_TOK_VALUE_TAG_START,
                                       NULL,
                                       row,
                                       col);
                    linked_list_append(tokens, tok);
                    i += delim_begin_len - 1;
                    col += delim_begin_len - 1;
                } else {
                    add_char_to_strval(&strval,
                                       &strval_bufsize,
                                       &strval_len,
                                       ch);
                    col += delim_begin_len - 1;
                }
            } else {
                add_char_to_strval(&strval, &strval_bufsize, &strval_len, ch);
                col += delim_begin_len - 1;
            }
        } else if (ch == delim_end[0]) {
            if (strncmp(str + i, delim_end, delim_end_len) == 0
                && str[i + delim_end_len] == delim_end[0]) {
                if (strval_len > 0) {
                    tok = token_create(CTACHE_TOK_STRING,
                                       strdup(strval),
                                       row,
                                       col);
                    strval_len = 0;
                    linked_list_append(tokens, tok);
                    memset(strval, 0, strval_bufsize);
                }
                tok = token_create(CTACHE_TOK_TAG_END, NULL, row, col);
                linked_list_append(tokens, tok);
                i += delim_end_len;
                col += delim_end_len;
            } else if (strncmp(str + i, delim_end, delim_end_len) == 0) {
                if (strval_len > 0) {
                    tok = token_create(CTACHE_TOK_STRING,
                                       strdup(strval),
                                       row,
                                       col);
                    strval_len = 0;
                    linked_list_append(tokens, tok);
                    memset(strval, 0, strval_bufsize);
                }
                tok = token_create(CTACHE_TOK_TAG_END, NULL, row, col);
                linked_list_append(tokens, tok);
                i += delim_end_len - 1;
                col += delim_end_len - 1;
            } else {
                add_char_to_strval(&strval, &strval_bufsize, &strval_len, ch);
                col += delim_end_len - 1;
            }
        } else if (ch == '\n') {
            add_char_to_strval(&strval, &strval_bufsize, &strval_len, ch);
            row++;
            col = 0;
        } else {
            add_char_to_strval(&strval, &strval_bufsize, &strval_len, ch);
            col += 1;
        }
    }
    if (strval_len > 0) {
        tok = token_create(CTACHE_TOK_STRING, strdup(strval), row, col);
        linked_list_append(tokens, tok);
        col += strval_len;
    }
    free(strval);
    strval = NULL;

    /* Add a final EOI token */
    tok = token_create(CTACHE_TOK_EOI, NULL, row, col);
    linked_list_append(tokens, tok);

    return tokens;
}

#undef STRVAL_INITIAL_BUFSIZE
