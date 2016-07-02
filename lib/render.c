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
#include "escaping.h"

#define IN_BUF_SIZE_DEFAULT 1024

/* text -> string */
static void
handle_text(struct linked_list_node **token_node_ptr, FILE *out, bool hidden)
{
    struct linked_list_node *token_node = *token_node_ptr;
    struct ctache_token *token_ptr = token_node->data;
    if (!hidden) {
        fprintf(out, "%s", token_ptr->value);
    }
    *token_node_ptr = (*token_node_ptr)->next;
}

/* tag start -> section tag start */
static void
handle_section_tag(struct linked_list_node **token_node_ptr,
                   ctache_data_t **curr_data_ptr,
                   struct linked_list *data_stack,
                   int *index_ptr,
                   struct linked_list *index_stack,
                   struct linked_list *token_node_stack,
                   struct linked_list *rule_node_stack,
                   struct linked_list_node *rule_node,
                   bool *hidden_ptr)
{
    *token_node_ptr = (*token_node_ptr)->next; /* Skip the {{# */

    struct ctache_token *token_ptr = (*token_node_ptr)->data;
    char *key = token_ptr->value;

    /* Change the data context to be the child data */
    if (ctache_data_is_hash(*curr_data_ptr)) {
        if (ctache_data_hash_table_has_key(*curr_data_ptr, key)) {
            linked_list_push(data_stack, *curr_data_ptr);
            int *ip = malloc(sizeof(int));
            *ip = *index_ptr;
            linked_list_push(index_stack, ip);

            ctache_data_t *child_data;
            child_data = ctache_data_hash_table_get(*curr_data_ptr, key);
            if (!ctache_data_is_boolean(child_data)) {
                *curr_data_ptr = child_data;
            } else {
                if (child_data->data.boolean == false) {
                    *hidden_ptr = true;
                }
            }
        } else {
            fprintf(stderr, "Key not in hash: %s\n", key);
        }
    } else if (ctache_data_is_array(*curr_data_ptr)) {
        linked_list_push(data_stack, *curr_data_ptr);
        int *ip = malloc(sizeof(int));
        *ip = *index_ptr;
        linked_list_push(index_stack, ip);
        if (ctache_data_array_get(*curr_data_ptr, *index_ptr) != NULL) {
            ctache_data_t *arr_data;
            arr_data = ctache_data_array_get(*curr_data_ptr, *index_ptr);
            if (ctache_data_is_hash(arr_data)) {
                if (ctache_data_hash_table_has_key(arr_data, key)) {
                    *curr_data_ptr = ctache_data_hash_table_get(arr_data, key);
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

    *token_node_ptr = (*token_node_ptr)->next; /* Move to the }} */
    *token_node_ptr = (*token_node_ptr)->next; /* Skip the }} */

    if (ctache_data_is_array(*curr_data_ptr)) {
        linked_list_push(token_node_stack, *token_node_ptr);
        linked_list_push(rule_node_stack, rule_node);
        *index_ptr = 0; /* Reset the array index */
    }
}

/* tag start -> close tag start */
static void
handle_close_tag(struct linked_list_node **token_node_ptr,
                 struct linked_list *token_node_stack,
                 ctache_data_t **curr_data_ptr,
                 struct linked_list_node **rule_node_ptr,
                 struct linked_list *rule_node_stack,
                 struct linked_list *data_stack,
                 int *index_ptr,
                 struct linked_list *index_stack,
                 bool *hidden_ptr)
{
    *token_node_ptr = (*token_node_ptr)->next; /* Skip the {{/ */
    int max_index = ctache_data_length(*curr_data_ptr) - 1;
    struct ctache_token *token = (*token_node_ptr)->data;
    char *key = token->value;

    if (token_node_stack->length > 0
            && ctache_data_is_array(*curr_data_ptr)
            && *index_ptr < max_index) {
        *token_node_ptr = linked_list_peek(token_node_stack);
        *rule_node_ptr = linked_list_peek(rule_node_stack);
        (*index_ptr)++;
    } else if (token_node_stack->length > 0
            && ctache_data_is_array(*curr_data_ptr)
            && *index_ptr >= max_index) {
        linked_list_pop(token_node_stack);
        linked_list_pop(rule_node_stack);
        if (data_stack->length > 0) {
            *curr_data_ptr = linked_list_pop(data_stack);
            int *ip = linked_list_pop(index_stack);
            *index_ptr = *ip;
            free(ip);
        }
        *token_node_ptr = (*token_node_ptr)->next; /* Move to the }} */
        *token_node_ptr = (*token_node_ptr)->next; /* Skip the }} */
    } else {
        ctache_data_t *child_data = NULL;
        if (ctache_data_is_hash(*curr_data_ptr)
            && ctache_data_hash_table_has_key(*curr_data_ptr, key)) {
            child_data = ctache_data_hash_table_get(*curr_data_ptr, key);
            if (ctache_data_is_boolean(child_data)
                && *hidden_ptr == true
                && child_data->data.boolean == false) {
                *hidden_ptr = false;
            }
        }
        if (data_stack->length > 0) {
            *curr_data_ptr = linked_list_pop(data_stack);
        }
        *token_node_ptr = (*token_node_ptr)->next; /* Move to the }} */
        *token_node_ptr = (*token_node_ptr)->next; /* Skip the }} */
    }
}

/* tag start -> value tag start */
static void
handle_value_tag(struct linked_list_node **token_node_ptr,
                       ctache_data_t *curr_data,
                       FILE *out,
                       int *index_ptr,
                       enum escaping_type escaping_type,
                       bool hidden)
{
    *token_node_ptr = (*token_node_ptr)->next; /* Skip the {{ */

    struct ctache_token *token_ptr = (*token_node_ptr)->data;
    char *key = token_ptr->value;
    ctache_data_t *value_data;
    char *str = NULL;

    if (ctache_data_is_hash(curr_data)) {
        value_data = ctache_data_hash_table_get(curr_data, key);
        if (value_data != NULL) {
            str = value_data->data.string;
        } else {
            fprintf(stderr, "Key not in hash: %s\n", token_ptr->value);
        }
    } else if (ctache_data_is_array(curr_data)) {
        if (token_ptr->value != NULL && key[0] == '.') {
            /* Immediate value from the array */
            ctache_data_t *str_data;
            str_data = ctache_data_array_get(curr_data, *index_ptr);
            str = str_data->data.string;
        } else {
            ctache_data_t *arr_data;
            arr_data = ctache_data_array_get(curr_data, *index_ptr);
            if (arr_data != NULL && ctache_data_is_hash(arr_data)) {
                ctache_data_t *str_data;
                if (ctache_data_hash_table_has_key(arr_data, key)) {
                    str_data = ctache_data_hash_table_get(arr_data, key);
                    str = str_data->data.string;
                } else {
                    fprintf(stderr, "Key not in hash: %s\n", key);
                }
            }
        }
    }

    bool escaped = escaping_type != ESCAPE_NONE;
    if (escaped && str != NULL) {
        str = escape_text(str, escaping_type);
    }

    if (str != NULL && !hidden) {
        fprintf(out, "%s", str);
    }

    if (escaped && str != NULL) {
        free(str);
        str = NULL;
    }

    *token_node_ptr = (*token_node_ptr)->next; /* Move on to the }} */
    *token_node_ptr = (*token_node_ptr)->next; /* Skip the }} */
}

static void
handle_partial(struct linked_list_node **token_node_ptr,
               ctache_data_t *curr_data,
               FILE *out,
               int *index_ptr,
               enum escaping_type escaping_type)
{
    (*token_node_ptr) = (*token_node_ptr)->next; /* Skip the {{> */

    struct ctache_token *token = (*token_node_ptr)->data;
    if (ctache_data_is_array(curr_data)) {
        curr_data = ctache_data_array_get(curr_data, *index_ptr);
        *index_ptr += 1;
    }

    if (!ctache_data_is_hash(curr_data)) {
        fprintf(stderr, "ERROR: current data is not a hash\n");
        abort();
    }

    /* Render what's inside the partial, then continue */
    char *key = token->value;
    if (ctache_data_hash_table_has_key(curr_data, key)) {
        ctache_data_t *partial_data;
        partial_data = ctache_data_hash_table_get(curr_data, key);
        char *partial = partial_data->data.string;
        ctache_render_string(partial,
                             strlen(partial),
                             out,
                             curr_data,
                             0, /* flags */
                             escaping_type);
    } else {
        fprintf(stderr, "ERROR: Key missing from hash: %s\n", key);
    }

    (*token_node_ptr) = (*token_node_ptr)->next; /* Skip past the string */
    (*token_node_ptr) = (*token_node_ptr)->next; /* Skip the }} */
}

static void
_ctache_render(struct linked_list *tokens,
               struct linked_list *parsed_rules,
               FILE *out,
               ctache_data_t *data,
               enum escaping_type escaping_type)
{
    struct linked_list_node *rule_node;
    struct linked_list_node *token_node;
    int *rule_ptr;
    struct linked_list *data_stack;
    struct linked_list *token_node_stack;
    struct linked_list *rule_node_stack;
    struct linked_list *index_stack;
    ctache_data_t *curr_data;
    struct linked_list_node *curr;
    int index; /* Array index */
    bool hidden;

    curr_data = data;
    data_stack = linked_list_create();
    token_node_stack = linked_list_create();
    rule_node_stack = linked_list_create();
    index_stack = linked_list_create();
    hidden = false;

    token_node = tokens->first;
    for (rule_node = parsed_rules->first;
         rule_node != NULL;
         rule_node = rule_node->next) {
        rule_ptr = rule_node->data;
        switch(*rule_ptr) {
        case 3: /* text -> string */
            handle_text(&token_node, out, hidden);
            break;
        case 6: /* tag start -> section tag start */
            handle_section_tag(&token_node,
                               &curr_data,
                               data_stack,
                               &index,
                               index_stack,
                               token_node_stack,
                               rule_node_stack,
                               rule_node,
                               &hidden);
            break;
        case 7: /* tag start -> close tag start */
            handle_close_tag(&token_node,
                             token_node_stack,
                             &curr_data,
                             &rule_node,
                             rule_node_stack,
                             data_stack,
                             &index,
                             index_stack,
                             &hidden);
            break;
        case 8: /* tag start -> value tag start */
            handle_value_tag(&token_node,
                             curr_data,
                             out,
                             &index,
                             escaping_type,
                             hidden);
            break;
        case 9: /* tag start -> unescaped value tag start */
            handle_value_tag(&token_node,
                             curr_data,
                             out,
                             &index,
                             ESCAPE_NONE,
                             hidden);
            break;
        case 10: /* tag start -> partial tag */
            handle_partial(&token_node,
                           curr_data,
                           out,
                           &index,
                           escaping_type);
            break;
        default:
            break;
        }
    }

    while (index_stack->length > 0) {
        void *ptr = linked_list_pop(index_stack);
        free(ptr);
    }
    linked_list_destroy(index_stack);
    linked_list_destroy(rule_node_stack);
    linked_list_destroy(token_node_stack);
    for (curr = data_stack->first; curr != NULL; curr = curr->next) {
        ctache_data_destroy((ctache_data_t *)curr->data);
    }
    linked_list_destroy(data_stack);
}

void
ctache_render_string(const char *in_str,
                     size_t in_str_len,
                     FILE *out_fp,
                     ctache_data_t *data,
                     int flags,
                     enum escaping_type escaping_type)
{
    struct linked_list *tokens = NULL;
    struct linked_list_node *curr = NULL;
    struct linked_list *parsed_rules = NULL;

    /* Flags */
    bool print_tokens = flags & CTACHE_RENDER_FLAG_PRINT_TOKENS;
    bool print_parsed_rules = flags & CTACHE_RENDER_FLAG_PRINT_RULES;

    /* Perform lexical analysis */
    tokens = ctache_lex(in_str, in_str_len);
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
        _ctache_render(tokens, parsed_rules, out_fp, data, escaping_type);
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
}

void
ctache_render_file(FILE *in_fp,
                   FILE *out_fp,
                   ctache_data_t *data,
                   int flags,
                   enum escaping_type escaping_type)
{
    char *in_buf = NULL;

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
            in_buf[in_buf_len] = ch;
            in_buf_len++;
        }
    }
    in_buf[in_buf_len] = '\0';

    ctache_render_string(in_buf,
                         in_buf_len,
                         out_fp,
                         data,
                         flags,
                         escaping_type);

    if (in_buf != NULL) {
        free(in_buf);
    }
}
