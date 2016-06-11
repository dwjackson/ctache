/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"
#include "lexer.h"

static int
parse_table[CTACHE_NUM_NONTERMINALS][CTACHE_NUM_TERMINALS] = {
    { 1, -1, 1, -1,  0, -1 },
    { 3,  3, 3,  3,  2,  3 },
    { 4,  4, 4, -1, -1, -1 },
    { 5,  6, 7, -1, -1, -1 }
};

static int
parse_table_check_match(enum ctache_token_type non_terminal,
                        enum ctache_token_type terminal)
{
    int match;
    int row = non_terminal - CTACHE_NUM_TERMINALS;
    int col = terminal;
    if (row < 0 || row >= CTACHE_NUM_NONTERMINALS) {
        match = -1;
    } else if (terminal >= CTACHE_NUM_TERMINALS) {
        match = -1;
    } else {
        match = parse_table[row][col];
    }
    return match;
}
    
struct linked_list
*ctache_parse(struct linked_list *tokens)
{
    struct linked_list *parsed_rules;
    struct linked_list_node *token_node;
    struct ctache_token *token;
    enum ctache_token_type *token_type_ptr;
    int match;
    void *ptr;
    int *rule_ptr;

    /* Set up the stack */
    struct linked_list *stack = linked_list_create();
    token_type_ptr = malloc(sizeof(*token_type_ptr));
    *token_type_ptr = CTACHE_TOK_EOI;
    linked_list_push(stack, token_type_ptr);
    *token_type_ptr = CTACHE_TOK_TEMPLATE;
    linked_list_push(stack, token_type_ptr);

    /* Set up the parsed rule list */
    parsed_rules = linked_list_create();

    /* Parse */
    token_node = tokens->first;
    while (token_node != NULL) {
        token = token_node->data;
        token_type_ptr = linked_list_peek(stack);
        match = parse_table_check_match(*token_type_ptr, token->tok_type);
        if (*token_type_ptr == token->tok_type) {
            ptr = linked_list_pop(stack);
            free(ptr);
            ptr = NULL;
            token_node = token_node->next;
        } else if (match >= 0) {
            ptr = linked_list_pop(stack);
            free(ptr);
            ptr = NULL;

            switch (match) { // TODO
            case 0: /* Rule 1: template -> text, template */
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_TEMPLATE;
                linked_list_push(stack, token_type_ptr);
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_TEXT;
                linked_list_push(stack, token_type_ptr);
                break;
            case 1: /* Rule 2: template -> tag, template */
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_TEMPLATE;
                linked_list_push(stack, token_type_ptr);
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_TAG;
                linked_list_push(stack, token_type_ptr);
                break;
            case 2: /* Rule 3: text -> string */
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_STRING;
                break;
            case 3: /* Rule 4: text -> "" */
                break;
            case 4: /* Rule 5: tag -> tag start, string, tag end */
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_TAG_END;
                linked_list_push(stack, token_type_ptr);
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_STRING;
                linked_list_push(stack, token_type_ptr);
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_TAG_START;
                linked_list_push(stack, token_type_ptr);
                break;
            case 5: /* Rule 6: tag start -> section tag start */
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_SECTION_TAG_START;
                linked_list_push(stack, token_type_ptr);
                break;
            case 6: /* Rule 7: tag start -> close tag start */
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_CLOSE_TAG_START;
                linked_list_push(stack, token_type_ptr);
                break;
            case 7: /* Rule 8: tag start -> value tag start */
                token_type_ptr = malloc(sizeof(*token_type_ptr));
                *token_type_ptr = CTACHE_TOK_VALUE_TAG_START;
                linked_list_push(stack, token_type_ptr);
                break;
            default:
                break;
            }
            rule_ptr = malloc(sizeof(*rule_ptr));
            *rule_ptr = match;
            linked_list_push(parsed_rules, rule_ptr);
        } else {
            fprintf(stderr, "Syntax error\n");
            linked_list_destroy(parsed_rules);
            parsed_rules = NULL;
            goto cleanup;
        }
    }
    
    /* Cleanup */
cleanup:
    linked_list_destroy(stack);
    
    return parsed_rules;
}
