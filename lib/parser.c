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
    int row = non_terminal - CTACHE_NUM_NONTERMINALS;
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
            token_node = token_node->next;
        } else if (match >= 0) {
            // TODO
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
