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
    { 1, 1, 1, -1,  0,  2 },
    { 4, 4, 4,  4,  3,  4 },
    { 5, 5, 5, -1, -1, -1 },
    { 6, 7, 8, -1, -1, -1 }
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

static void
push_token_onto_stack(struct linked_list *stack,
                      enum ctache_token_type token_type)
{
    enum ctache_token_type *token_type_ptr;
    token_type_ptr = malloc(sizeof(*token_type_ptr));
    *token_type_ptr = token_type;
    linked_list_push(stack, token_type_ptr);
}

static void
pop_token_off_stack(struct linked_list *stack)
{
    void *ptr;
    ptr = linked_list_pop(stack);
    free(ptr);
    ptr = NULL;
}

static void
append_parsed_rule(struct linked_list *parsed_rules, int rule_index)
{
    int *rule_ptr;
    rule_ptr = malloc(sizeof(*rule_ptr));
    *rule_ptr = rule_index;
    linked_list_append(parsed_rules, rule_ptr);
}

struct linked_list
*ctache_parse(struct linked_list *tokens)
{
    struct linked_list *parsed_rules;
    struct linked_list_node *token_node;
    struct ctache_token *token;
    enum ctache_token_type *token_type_ptr;
    int match;
    struct linked_list *stack;

    /* Set up the stack */
    stack = linked_list_create();

    push_token_onto_stack(stack, CTACHE_TOK_EOI);
    push_token_onto_stack(stack, CTACHE_TOK_TEMPLATE);

    /* Set up the parsed rule list */
    parsed_rules = linked_list_create();

    /* Parse */
    token_node = tokens->first;
    while (token_node != NULL) {
        token = token_node->data;
        token_type_ptr = linked_list_peek(stack);
        match = parse_table_check_match(*token_type_ptr, token->tok_type);
        if (*token_type_ptr == token->tok_type) {
            pop_token_off_stack(stack);
            token_node = token_node->next;
        } else if (match >= 0) {
            pop_token_off_stack(stack);

            switch (match) {
            case 0: /* Rule 1: template -> text, template */
                push_token_onto_stack(stack, CTACHE_TOK_TEMPLATE);
                push_token_onto_stack(stack, CTACHE_TOK_TEXT);
                break;
            case 1: /* Rule 2: template -> tag, template */
                push_token_onto_stack(stack, CTACHE_TOK_TEMPLATE);
                push_token_onto_stack(stack, CTACHE_TOK_TAG);
                break;
            case 2: /* Rule 3: template -> "" */
                break;
            case 3: /* Rule 4: text -> string */
                push_token_onto_stack(stack, CTACHE_TOK_STRING);
                break;
            case 4: /* Rule 5: text -> "" */
                break;
            case 5: /* Rule 6: tag -> tag start, string, tag end */
                push_token_onto_stack(stack, CTACHE_TOK_TAG_END);
                push_token_onto_stack(stack, CTACHE_TOK_STRING);
                push_token_onto_stack(stack, CTACHE_TOK_TAG_START);
                break;
            case 6: /* Rule 7: tag start -> section tag start */
                push_token_onto_stack(stack, CTACHE_TOK_SECTION_TAG_START);
                break;
            case 7: /* Rule 8: tag start -> close tag start */
                push_token_onto_stack(stack, CTACHE_TOK_CLOSE_TAG_START);
                break;
            case 8: /* Rule 9: tag start -> value tag start */
                push_token_onto_stack(stack, CTACHE_TOK_VALUE_TAG_START);
                break;
            default:
                break;
            }
            append_parsed_rule(parsed_rules, match);
        } else {
            fprintf(stderr, "Syntax error\n");

            enum ctache_token_type tok_type = token->tok_type;
            extern char *ctache_token_names[];
            char *token_name = ctache_token_names[tok_type];
            char *value = token->value;
            char *stack_token_name = ctache_token_names[*token_type_ptr];
            struct linked_list_node *curr;
            fprintf(stderr, "token = (%s, \"%s\")\n", token_name, value);
            fprintf(stderr, "stack top = %s\n", stack_token_name);
            fprintf(stderr, "Parsed rules: ");
            for (curr = parsed_rules->first; curr != NULL; curr = curr->next) {
                fprintf(stderr, "%d", *((int*)curr->data));
                if (curr->next != NULL) {
                    fprintf(stderr, ", ");
                }
            }
            fprintf(stderr, "\n");

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
