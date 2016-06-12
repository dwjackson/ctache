/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <stdbool.h>
#include "ctache_data.h"
#include "linked_list.h"
#include "lexer.h"
#include "render.h"
#include "parser.h"

#define IN_BUF_SIZE_DEFAULT 1024

static void
_ctache_render(struct linked_list *tokens,
              struct linked_list *parsed_rules,
              FILE *out,
              ctache_data_t *data)
{
    struct linked_list_node *rule_node;
    struct linked_list_node *token_node;
    struct ctache_token *token_ptr;
    int *rule_ptr;
    ctache_data_t *value_data;
    char *str;

    token_node = tokens->first;
    for (rule_node = parsed_rules->first;
         rule_node != NULL;
         rule_node = rule_node->next) {
        rule_ptr = rule_node->data;
        switch(*rule_ptr) {
        case 0: /* template -> text, template */
            break;
        case 1: /* template -> tag, template */
            break;
        case 2: /* template -> "" */
            break;
        case 3: /* text -> string */
            token_ptr = token_node->data;
            fprintf(out, "%s", token_ptr->value);
            token_node = token_node->next;
            break;
        case 4: /* text -> "" */
            break;
        case 5: /* tag -> tag start, string, tag end */
            break;
        case 6: /* tag start -> section tag start */
            // TODO
            break;
        case 7: /* tag start -> close tag start */
            // TODO
            break;
        case 8: /* tag start -> value tag start */
            token_node = token_node->next; /* Skip the {{ */

            token_ptr = token_node->data;
            value_data = ctache_data_hash_table_get(data, token_ptr->value);
            str = value_data->data.string;
            fprintf(out, "%s", str);

            token_node = token_node->next; /* Move on to the }} */
            token_node = token_node->next; /* Skip the }} */
            break;
        default:
            break;
        }
    }
}

void
ctache_render_file(FILE *in_fp, FILE *out_fp, ctache_data_t *data, int flags)
{
    struct linked_list *tokens = NULL;
    char *in_buf = NULL;
    struct linked_list_node *curr = NULL;
    struct linked_list *parsed_rules = NULL;

    /* Flags */
    bool print_tokens = flags & CTACHE_RENDER_FLAG_PRINT_TOKENS;
    bool print_parsed_rules = flags & CTACHE_RENDER_FLAG_PRINT_RULES;

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
            enum ctache_token_type tok_type = tok->tok_type;
            printf("\t%s (%d)", ctache_token_names[tok_type], tok_type);
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
    if (print_parsed_rules && parsed_rules != NULL) {
        printf("Parsed rules:\n");
        for (curr = parsed_rules->first; curr != NULL; curr = curr->next) {
            int *rule_ptr = curr->data;
            printf("\t%d\n", *rule_ptr);
        }
        goto cleanup; /* Do nothing else */
    }

    /* Render the template to the file */
    if (tokens != NULL && parsed_rules != NULL) {
        _ctache_render(tokens, parsed_rules, out_fp, data);
    }

cleanup:
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
}
