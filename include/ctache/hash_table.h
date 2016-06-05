/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "linked_list.h"

struct ctache_hash_table {
    int bufsize; /* Total size of the "cells" buffer */
    int length; /* Number of elements */
    struct linked_list *cells[];
};

struct ctache_hash_table
*ctache_hash_table_create();

void
ctache_hash_table_destroy(struct ctache_hash_table *table,
                          void (*free_data)(void *));

void
ctache_hash_table_set(struct ctache_hash_table *table,
                      const char *key,
                      void *value);

#endif /* HASH_TABLE */
