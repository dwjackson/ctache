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
    char *yaml_file_name = NULL;
    ctache_data_t *data = NULL;
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

    /* Read the YAML data file */
    data = data_from_yaml(yaml_file_name);

    /* Render the template */
    if (data != NULL) {
        int render_flags = 0x0;
        if (print_tokens) {
            render_flags |= CTACHE_RENDER_FLAG_PRINT_TOKENS;
        }
        if (print_parsed_rules) {
            render_flags |= CTACHE_RENDER_FLAG_PRINT_RULES;
        }
        ctache_render_file(in_fp, out_fp, data, render_flags);
    } else {
        fprintf(stderr, "Error parsing YAML file\n");
    }
    
cleanup:
    /* Cleanup */
    if (data != NULL) {
        ctache_data_destroy(data);
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
