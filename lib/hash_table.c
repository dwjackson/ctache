/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "linked_list.h"
#include "hash_table.h"

#define DEFAULT_BUFSIZE 10

struct ctache_hash_table_cell {
    char *key;
    void *value;
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

static uint32_t
sdbm_hash(const char *str)
{
    uint32_t hash = 0x0;
    int ch;

    while ((ch = *str++) != '\0') {
        hash = ch + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

void
ctache_hash_table_set(struct ctache_hash_table *table,
                      const char *key,
                      void *value)
{
    uint32_t hash = sdbm_hash(key);
    uint32_t index = hash % table->bufsize;
    struct linked_list *list = (table->cells)[index];
    linked_list_append(list, value);
}

#undef DEFAULT_BUFSIZE 
