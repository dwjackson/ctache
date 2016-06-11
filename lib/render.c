/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include "linked_list.h"
#include "lexer.h"
#include "ctache_data.h"

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
    char *value;

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
            token_ptr = token_node->data;
            value = ctache_data_hash_table_get(data, token_ptr->value);
            fprintf(out, "%s", value);
            token_node = token_node->next; /* Skip the {{ */
            token_node = token_node->next; /* Skip the string */
            token_node = token_node->next; /* Skip the }} */
            break;
        default:
            break;
        }
    }
}

void
ctache_render_file(FILE *in, FILE *out, ctache_data_t *data)
{
    // TODO
}
