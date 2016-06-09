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
#include "parser.h"

#define DEFAULT_BUFSIZE 10

struct ctache_hash_table
*ctache_hash_table_create()
{
    size_t size = sizeof(struct ctache_hash_table)
        + DEFAULT_BUFSIZE * sizeof(struct linked_list *);
    struct ctache_hash_table *table = malloc(size);
    int i;
    if (table != NULL) {
        memset(table, 0, size); /* Set every list to NULL */
        table->bufsize = DEFAULT_BUFSIZE;
        if (table->bufsize == 0) {
            abort();
        }
        table->length = 0;

        /* Initialize the linked lists */
        for (i = 0; i < table->bufsize; i++) {
            (table->cells)[i] = linked_list_create();
        }
    }
    return table;
}

void
ctache_hash_table_destroy(struct ctache_hash_table *table,
                          void (*free_data)(void *))
{
    struct linked_list *list;
    struct linked_list_node *curr;
    struct ctache_hash_table_cell *cell;
    int i;
    for (i = 0; i < table->bufsize; i++) {
        list = (table->cells)[i];
        if (list != NULL) {
            for (curr = list->first; curr != NULL; curr = curr->next) {
                cell = curr->data;
                void *data = cell->value;
                free_data(data);
                free(cell->key);
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
    struct ctache_hash_table_cell *cell = malloc(sizeof(*cell));
    cell->key = strdup(key);
    cell->value = value;
    linked_list_append(list, cell);
}

void
*ctache_hash_table_get(struct ctache_hash_table *table, const char *key)
{
    void *value = NULL;
    uint32_t hash = sdbm_hash(key);
    uint32_t index = hash % table->bufsize;
    struct linked_list *list = (table->cells)[index];
    struct linked_list_node *curr = list->first;
    struct ctache_hash_table_cell *cell;
    while (curr != NULL) {
        cell = (struct ctache_hash_table_cell *)(curr->data);
        if (strcmp(cell->key, key) == 0) {
            value = cell->value;
            break;
        }
        curr = curr->next;
    }
    return value;
}

#undef DEFAULT_BUFSIZE 
