/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "ctache.h"

#define IN_BUF_SIZE_DEFAULT 1024

int
main(int argc, char *argv[])
{
    FILE *in_fp = stdin; /* Input file name */
    char *in_file_name = NULL; /* Input file name */
    FILE *out_fp = stdout; /* Output file pointer */
    char *out_file_name = NULL; /* Output file name */
    bool print_tokens = false;
    bool print_help = false;
    struct linked_list_node *curr = NULL;
    struct linked_list *tokens = NULL;
    char *in_buf = NULL;

    extern char *optarg;
    extern int optind, opterr, optopt;
    int opt;
    while ((opt = getopt(argc, argv, "o:i:th")) != -1) {
        switch (opt) {
        case 'o':
            out_file_name = strdup(optarg);
            out_fp = fopen(out_file_name, "w");
            if (out_fp == NULL) {
                fprintf(stderr, "Error opening file: %s\n", out_file_name);
                exit(EXIT_FAILURE);
            }
            break;
        case 'i':
            in_file_name = strdup(optarg);
            in_fp = fopen(in_file_name, "r");
            if (in_fp == NULL) {
                fprintf(stderr, "Error opening file: %s\n", in_file_name);
                exit(EXIT_FAILURE);
            }
            break;
        case 't':
            print_tokens = true;
            break;
        case 'h':
            print_help = true;
            break;
        default:
            printf("Unrecognized option: %c\n", opt);
            exit(EXIT_FAILURE);
            break;
        }
    }

    if (print_help) {
        printf("Usage: %s [OPTIONS] ...\n", argv[0]);
        printf("\t-h: print this help message\n");
        printf("\t-t: only print lexer tokens, do not parse\n");
        printf("\t-i: specify input file name\n");
        printf("\t-o: Specify output file name\n");
        goto cleanup; /* Do nothing else */
    }
    
    /* Read the input file into a string */
    size_t in_buf_size = IN_BUF_SIZE_DEFAULT;
    size_t in_buf_len = 0;
    in_buf = malloc(in_buf_size);
    if (in_buf == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    int ch;
    while ((ch = fgetc(in_fp)) != EOF) {
        if (in_buf_len + 1 < in_buf_size - 1) { /* -1 is for '\0') */
            in_buf[in_buf_len] = ch;
            in_buf_len++;
        } else {
            in_buf_size *= 2;
            in_buf = realloc(in_buf, in_buf_len);
            if (in_buf == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    in_buf[in_buf_len] = '\0';

    /* Perform lexical analysis */
    tokens = ctache_lex(in_buf, in_buf_len);
    if (print_tokens) {
        extern char *ctache_token_names[];
        printf("Tokens:\n");
        for (curr = tokens->first; curr != NULL; curr = curr->next) {
            struct ctache_token *tok = (struct ctache_token *)(curr->data);
            printf("\t%s", ctache_token_names[tok->tok_type]);
            if (tok->tok_type == CTACHE_TOK_STRING) {
                printf(" \"%s\"", tok->value);
            }
            printf("\n");
        }
        goto cleanup; /* Do not perform actual parsing */
    }

    // TODO: Parsing
    
cleanup:
    /* Cleanup */
    if (tokens != NULL) {
        for (curr = tokens->first; curr != NULL; curr = curr->next) {
            struct ctache_token *tok = (struct ctache_token *)(curr->data);
            if (tok->value != NULL) {
                free(tok->value);
            }
            free(tok);
        }
        linked_list_destroy(tokens);
    }
    if (in_buf != NULL) {
        free(in_buf);
    }
    if (out_fp != stdout) {
        fclose(out_fp);
    }
    if (in_fp != stdin) {
        fclose(in_fp);
    }
    if (in_file_name != NULL) {
        free(in_file_name);
    }
    if (out_file_name != NULL) {
        free(out_file_name);
    }

    return 0;
}
