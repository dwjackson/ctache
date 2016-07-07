/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#ifndef CTACHE_HASH_TABLE_H
#define CTACHE_HASH_TABLE_H

#include "linked_list.h"

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
*ctache_hash_table_create();

void
ctache_hash_table_destroy(struct ctache_hash_table *table,
                          void (*free_data)(void *));

void
ctache_hash_table_set(struct ctache_hash_table *table,
                      const char *key,
                      void *value);

void
*ctache_hash_table_get(struct ctache_hash_table *table, const char *key);

bool
ctache_hash_table_has_key(struct ctache_hash_table *table, const char *key);

#endif /* CTACHE_HASH_TABLE */
