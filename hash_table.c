#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

#define DEFAULT_BUFSIZE 10

struct ctache_hash_table_cell {
    char *key;
    void *value;
};

struct ctache_hash_table {
    int bufsize; /* Total size of the "cells" buffer */
    int length; /* Number of elements */
    struct linked_list *cells[];
};

struct ctache_hash_table
*ctache_hash_table_create()
{
    size_t size = sizeof(struct ctache_hash_table)
        + DEFAULT_BUFSIZE * sizeof(struct linked_list *);
    struct ctache_hash_table *table = malloc(size);
    if (table != NULL) {
        memset(table, 0, size); /* Set every list to NULL */
    }
    return table;
}

void
ctache_hash_table_destroy(struct ctache_hash_table *table,
                          void (*free_data)(void *))
{
    struct linked_list *list;
    struct linked_list_node *curr;
    int i;
    for (i = 0; i < table->bufsize; i++) {
        list = (table->cells)[i];
        if (list != NULL) {
            for (curr = list->first; curr != NULL; curr = curr->next) {
                free_data(curr->data);
            }
            linked_list_destroy(list);
        }
    }
    free(table);
}

void
ctache_hash_table_set(struct ctache_hash_table *table,
                      const char *key,
                      void *value)
{
    // TODO
}

#undef DEFAULT_BUFSIZE 
