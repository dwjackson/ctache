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
#include <yaml.h>
#include "ctache.h"
#include "yaml_data.h"

#define IN_BUF_SIZE_DEFAULT 1024

void
print_help(const char* prog_name);

int
main(int argc, char *argv[])
{
    FILE *in_fp = stdin; /* Input file name */
    char *in_file_name = NULL; /* Input file name */
    FILE *out_fp = stdout; /* Output file pointer */
    char *out_file_name = NULL; /* Output file name */
    bool print_tokens = false;
    bool help_flag_set = false;
    struct linked_list_node *curr = NULL;
    struct linked_list *tokens = NULL;
    char *in_buf = NULL;
    char *yaml_file_name = NULL;
    ctache_data_t *data = NULL;
    struct linked_list *parsed_rules = NULL;
    bool print_parsed_rules = false;

    extern char *optarg;
    extern int optind, opterr, optopt;
    int opt;
    while ((opt = getopt(argc, argv, "o:i:thy:p")) != -1) {
        switch (opt) {
        case 'h':
            help_flag_set = true;
            break;
        case 'i':
            in_file_name = strdup(optarg);
            in_fp = fopen(in_file_name, "r");
            if (in_fp == NULL) {
                fprintf(stderr, "Error opening file: %s\n", in_file_name);
                exit(EXIT_FAILURE);
            }
            break;
        case 'o':
            out_file_name = strdup(optarg);
            out_fp = fopen(out_file_name, "w");
            if (out_fp == NULL) {
                fprintf(stderr, "Error opening file: %s\n", out_file_name);
                exit(EXIT_FAILURE);
            }
            break;
        case 'p':
            print_parsed_rules = true;
            break;
        case 't':
            print_tokens = true;
            break;
        case 'y':
            yaml_file_name = strdup(optarg);
            break;
        default:
            printf("Unrecognized option: %c\n", opt);
            exit(EXIT_FAILURE);
            break;
        }
    }

    if (help_flag_set) {
        print_help(argv[0]);
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
        if (!print_parsed_rules) {
            goto cleanup; /* Do not perform actual parsing */
        }
    }

    parsed_rules = ctache_parse(tokens);
    if (print_parsed_rules) {
        printf("Parsed rules:\n");
        for (curr = parsed_rules->first; curr != NULL; curr = curr->next) {
            int *rule_ptr = curr->data;
            printf("\t%d\n", *rule_ptr);
        }
        goto cleanup; /* Do nothing else */
    }

    /* Read the YAML data file */
    data = data_from_yaml(yaml_file_name);

    // TODO: Render the template
    
cleanup:
    /* Cleanup */
    if (data != NULL) {
        ctache_data_destroy(data);
    }
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
    if (parsed_rules != NULL) {
        for (curr = parsed_rules->first; curr != NULL; curr = curr->next) {
            free(curr->data);
        }
        linked_list_destroy(parsed_rules);
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
    if (yaml_file_name != NULL) {
        free(yaml_file_name);
    }

    return 0;
}

void
print_help(const char *prog_name)
{
    printf("Usage: %s [OPTIONS] ...\n", prog_name);
    printf("\t-h: print this help message\n");
    printf("\t-t: print lexer tokens (for debugging)\n");
    printf("\t-i: specify input file name\n");
    printf("\t-o: Specify output file name\n");
    printf("\t-p: Print list of parsed rules (for debugging)\n");
    printf("\t-y: Specify the YAML-formatted input data file");
}
