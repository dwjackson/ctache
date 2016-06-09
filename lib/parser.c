#include <stdlib.h>
#include "linked_list.h"
#include "lexer.h"

static int
parse_table[CTACHE_NUM_NONTERMINALS][CTACHE_NUM_TERMINALS] = {
};
    
void
ctache_parse(struct linked_list *tokens)
{
    struct linked_list_node *curr;
    struct ctache_token *token;
    enum ctache_token_type *token_type;

    /* Set up the stack */
    struct linked_list *stack = linked_list_create();
    token_type = malloc(sizeof(*token_type));
    *token_type = CTACHE_TOK_EOI;
    linked_list_push(stack, token_type);
    *token_type = CTACHE_TOK_TEMPLATE;
    linked_list_push(stack, token_type);
    
    linked_list_destroy(stack);
}
