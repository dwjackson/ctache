/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ctache_data.h"
#include "linked_list.h"
#include "lexer.h"
#include "render.h"
#include "parser.h"

#define IN_BUF_SIZE_DEFAULT 1024

/* text -> string */
static void
handle_rule3(struct linked_list_node **token_node_ptr, FILE *out)
{
    struct linked_list_node *token_node = *token_node_ptr;
    struct ctache_token *token_ptr = token_node->data;
    fprintf(out, "%s", token_ptr->value);
    *token_node_ptr = (*token_node_ptr)->next;
}

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
    struct linked_list *data_stack;
    struct linked_list *token_node_stack;
    struct linked_list *rule_node_stack;
    struct linked_list *index_stack;
    ctache_data_t *curr_data;
    char *key;
    struct linked_list_node *curr;
    int index; /* Array index */

    curr_data = data;
    data_stack = linked_list_create();
    token_node_stack = linked_list_create();
    rule_node_stack = linked_list_create();
    index_stack = linked_list_create();

    token_node = tokens->first;
    for (rule_node = parsed_rules->first;
         rule_node != NULL;
         rule_node = rule_node->next) {
        rule_ptr = rule_node->data;
        switch(*rule_ptr) {
        case 3: /* text -> string */
            handle_rule3(&token_node, out);
            break;
        case 6: /* tag start -> section tag start */
            token_node = token_node->next; /* Skip the {{# */

            token_ptr = token_node->data;
            key = token_ptr->value;

            /* Change the data context to be the child data */
            if (curr_data->data_type == CTACHE_DATA_HASH) {
                if (ctache_data_hash_table_has_key(curr_data, key)) {
                    linked_list_push(data_stack, curr_data);
                    int *ip = malloc(sizeof(int));
                    *ip = index;
                    linked_list_push(index_stack, ip);
                    curr_data = ctache_data_hash_table_get(curr_data, key);
                } else {
                    fprintf(stderr, "Key not in hash: %s\n", key);
                }
            } else if (curr_data->data_type == CTACHE_DATA_ARRAY) {
                linked_list_push(data_stack, curr_data);
                int *ip = malloc(sizeof(int));
                *ip = index;
                linked_list_push(index_stack, ip);
                if (ctache_data_array_get(curr_data, index) != NULL) {
                    ctache_data_t *arr_data;
                    arr_data = ctache_data_array_get(curr_data, index);
                    if (arr_data->data_type == CTACHE_DATA_HASH) {
                        if (ctache_data_hash_table_has_key(arr_data, key)) {
                            curr_data = ctache_data_hash_table_get(arr_data,
                                                                   key);
                        } else {
                            fprintf(stderr, "Key is not in hash: %s\n", key);
                        }
                    } else {
                        fprintf(stderr, "Data in array is not a hash\n");
                    }
                }
            } else {
                fprintf(stderr, "Data is not a hash or array\n");
            }

            token_node = token_node->next; /* Move to the }} */
            token_node = token_node->next; /* Skip the }} */

            if (curr_data->data_type == CTACHE_DATA_ARRAY) {
                linked_list_push(token_node_stack, token_node);
                linked_list_push(rule_node_stack, rule_node);
                index = 0; /* Reset the array index */
            }
            break;
        case 7: /* tag start -> close tag start */
            token_node = token_node->next; /* Skip the {{/ */
            if (token_node_stack->length > 0
                    && ctache_data_is_array(curr_data)
                    && index < ctache_data_length(curr_data)) {
                token_node = linked_list_peek(token_node_stack);
                rule_node = linked_list_peek(rule_node_stack);
            } else if (token_node_stack->length > 0
                    && ctache_data_is_array(curr_data)
                    && index >= ctache_data_length(curr_data)) {
                linked_list_pop(token_node_stack);
                linked_list_pop(rule_node_stack);
                if (data_stack->length > 0) {
                    curr_data = linked_list_pop(data_stack);
                    int *ip = linked_list_pop(index_stack);
                    index = *ip;
                    free(ip);
                    index++;
                }
                token_node = token_node->next; /* Move to the }} */
                token_node = token_node->next; /* Skip the }} */
            } else {
                if (data_stack->length > 0) {
                    curr_data = linked_list_pop(data_stack);
                }
                token_node = token_node->next; /* Move to the }} */
                token_node = token_node->next; /* Skip the }} */
            }
            break;
        case 8: /* tag start -> value tag start */
            token_node = token_node->next; /* Skip the {{ */

            token_ptr = token_node->data;
            key = token_ptr->value;
            if (curr_data->data_type == CTACHE_DATA_HASH) {
                value_data = ctache_data_hash_table_get(curr_data, key);
                if (value_data != NULL) {
                    char *str = value_data->data.string;
                    fprintf(out, "%s", str);
                } else {
                    char *err_fmt = "Key missing from hash: \"%s\"\n";
                    fprintf(stderr, err_fmt, token_ptr->value);
                    goto cleanup;
                }
            } else if (curr_data->data_type == CTACHE_DATA_ARRAY) {
                if (token_ptr->value != NULL && key[0] == '.') {
                    /* Immediate value from the array */
                    ctache_data_t *str_data;
                    str_data = ctache_data_array_get(curr_data, index);
                    fprintf(out, "%s", str_data->data.string);
                    index++;
                } else {
                    ctache_data_t *arr_data = ctache_data_array_get(curr_data,
                                                                    index);
                    if (arr_data != NULL
                            && arr_data->data_type == CTACHE_DATA_HASH) {
                        ctache_data_t *str_data;
                        if (ctache_data_hash_table_has_key(arr_data, key)) {
                            str_data = ctache_data_hash_table_get(arr_data,
                                                                  key);
                            fprintf(out, "%s", str_data->data.string);
                        } else {
                            fprintf(stderr, "Key not in hash: %s\n", key);
                        }
                    }
                }
            }

            token_node = token_node->next; /* Move on to the }} */
            token_node = token_node->next; /* Skip the }} */
            break;
        default:
            break;
        }
    }

cleanup:
    linked_list_destroy(index_stack);
    linked_list_destroy(rule_node_stack);
    linked_list_destroy(token_node_stack);
    for (curr = data_stack->first; curr != NULL; curr = curr->next) {
        ctache_data_destroy((ctache_data_t *)curr->data);
    }
    linked_list_destroy(data_stack);
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
            in_buf = realloc(in_buf, in_buf_size);
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
                int i;
                size_t value_len = strlen(tok->value);
                char ch;
                printf(" \"");
                for (i = 0; i < value_len; i++) {
                    ch = (tok->value)[i];
                    if (ch != '\n') {
                        printf("%c", ch);
                    } else {
                        printf("\\n");
                    }
                }
                printf("\"");
            }
            printf("\n");
        }
        if (!print_parsed_rules) {
            goto cleanup; /* Do not perform actual parsing */
        }
    }

    parsed_rules = ctache_parse(tokens);
    if (print_parsed_rules && parsed_rules != NULL) {
        int index = 0;
        printf("Parsed rules:\n");
        printf("\t");
        for (curr = parsed_rules->first; curr != NULL; curr = curr->next) {
            int *rule_ptr = curr->data;
            printf("%d", *rule_ptr);
            if (curr->next != NULL) {
                printf(", ");
            }
            index++;
            if (index % 10 == 0) {
                printf("\n\t");
            }
        }
        printf("\n");
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
