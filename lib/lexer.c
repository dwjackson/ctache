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

char *ctache_token_names[] = {
    "CTACHE_TOK_SECTION_TAG_START",
    "CTACHE_TOK_CLOSE_TAG_START",
    "CTACHE_TOK_VALUE_TAG_START",
    "CTACHE_TOK_TAG_END",
    "CTACHE_TOK_STRING",
    "CTACHE_TOK_EOI"
};

static void
add_char_to_strval(char *strval,
                   size_t *strval_bufsize_ptr,
                   size_t *strval_len_ptr,
                   int ch)
{
    if (*strval_len_ptr + 1 < *strval_bufsize_ptr - 1) {
        strval[*strval_len_ptr] = ch;
        (*strval_len_ptr)++;
    } else {
        (*strval_bufsize_ptr) *= 2;
        char *curr_strval = strdup(strval);
        if (strval == NULL) {
            fprintf(stderr, "ctache_lex(): Out of memory\n");
            exit(EXIT_FAILURE);
        }
        strval = realloc(strval, *strval_bufsize_ptr);
        memset(strval, 0, *strval_bufsize_ptr);
        strcpy(strval, curr_strval);
        free(curr_strval);
        if (strval == NULL) {
            fprintf(stderr, "ctache_lex(): Out of memory\n");
            exit(EXIT_FAILURE);
        }
        strval[*strval_len_ptr] = ch;
        (*strval_len_ptr)++;
    }
}

static void
oom_error(const char* func_name)
{
    fprintf(stderr, "%s(): Out of memory", func_name);
    exit(EXIT_FAILURE);
}

struct linked_list
*ctache_lex(const char *str, size_t str_len)
{
    struct linked_list *tokens = linked_list_create();
    if (tokens == NULL) {
        return NULL;
    }

    size_t strval_len = 0;
    size_t strval_bufsize = 10;
    char *strval = malloc(strval_bufsize);

    int ch, i;
    for (i = 0; i < str_len; i++) {
        ch = str[i];
        switch (ch) {
        case '{':
            if (i + 3 < str_len) {
                if (str[i + 1] == '{' && str[i + 2] == '#') {
                    struct ctache_token *tok = malloc(sizeof(*tok));
                    if (tok == NULL) {
                        oom_error("ctache_lex");
                    }
                    if (strval_len > 0) {
                        tok->tok_type = CTACHE_TOK_STRING;
                        tok->value = strdup(strval);
                        strval_len = 0;
                        memset(strval, 0, strval_bufsize);
                        linked_list_append(tokens, tok);
                        tok = malloc(sizeof(*tok));
                        if (tok == NULL) {
                            oom_error("ctache_lex");
                        }
                    }
                    tok->tok_type = CTACHE_TOK_SECTION_TAG_START;
                    tok->value = NULL;
                    linked_list_append(tokens, tok);
                    i += 2;
                } else if (str[i + 1] == '{' && str[i + 2] == '/') {
                    struct ctache_token *tok = malloc(sizeof(*tok));
                    if (tok == NULL) {
                        oom_error("ctache_lex");
                    }
                    if (strval_len > 0) {
                        tok->tok_type = CTACHE_TOK_STRING;
                        tok->value = strdup(strval);
                        strval_len = 0;
                        linked_list_append(tokens, tok);
                        memset(strval, 0, strval_bufsize);
                        tok = malloc(sizeof(*tok));
                        if (tok == NULL) {
                            oom_error("ctache_lex");
                        }
                    }
                    tok->tok_type = CTACHE_TOK_CLOSE_TAG_START;
                    tok->value = NULL;
                    linked_list_append(tokens, tok);
                    i += 2;
                } else if (str[i + 1] == '{') {
                    struct ctache_token *tok = malloc(sizeof(*tok));
                    if (tok == NULL) {
                        oom_error("ctache_lex");
                    }
                    if (strval_len > 0) {
                        tok->tok_type = CTACHE_TOK_STRING;
                        tok->value = strdup(strval);
                        strval_len = 0;
                        linked_list_append(tokens, tok);
                        memset(strval, 0, strval_bufsize);
                        tok = malloc(sizeof(*tok));
                    }
                    tok->tok_type = CTACHE_TOK_VALUE_TAG_START;
                    tok->value = NULL;
                    linked_list_append(tokens, tok);
                    i += 1;
                } else {
                    add_char_to_strval(strval,
                                       &strval_bufsize,
                                       &strval_len,
                                       ch);
                }
            } else {
                add_char_to_strval(strval, &strval_bufsize, &strval_len, ch);
            }
            break;
        case '}':
            if (i + 1 < str_len && str[i + 1] == '}') {
                struct ctache_token *tok = malloc(sizeof(*tok));
                if (tok == NULL) {
                    oom_error("ctache_lex");
                }
                if (strval_len > 0) {
                    tok->tok_type = CTACHE_TOK_STRING;
                    tok->value = strdup(strval);
                    strval_len = 0;
                    linked_list_append(tokens, tok);
                    memset(strval, 0, strval_bufsize);
                    tok = malloc(sizeof(*tok));
                }
                tok->tok_type = CTACHE_TOK_TAG_END;
                tok->value = NULL;
                linked_list_append(tokens, tok);
                i += 1;
            } else {
                add_char_to_strval(strval, &strval_bufsize, &strval_len, ch);
            }
            break;
        default:
            add_char_to_strval(strval, &strval_bufsize, &strval_len, ch);
            break;
        }
    }
    if (strval_len > 0) {
        struct ctache_token *tok = malloc(sizeof(*tok));
        if (tok == NULL) {
            oom_error("ctache_lex");
        }
        tok->tok_type = CTACHE_TOK_STRING;
        tok->value = strdup(strval);
        linked_list_append(tokens, tok);
    }

    free(strval);

    return tokens;
}
