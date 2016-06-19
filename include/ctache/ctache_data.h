/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CTACHE_DATA_H
#define CTACHE_DATA_H

#include <stdbool.h>

enum ctache_data_type {
    CTACHE_DATA_HASH,
    CTACHE_DATA_ARRAY,
    CTACHE_DATA_STRING,
    CTACHE_DATA_NUMBER,
    CTACHE_DATA_BOOLEAN
};

struct ctache_data {
    enum ctache_data_type data_type;
    union {
        struct ctache_hash_table *hash;
        struct ctache_array *array;
        char *string;
        double number;
        bool boolean;
    } data;
};

typedef struct ctache_data ctache_data_t;

ctache_data_t
*ctache_data_create_array(size_t element_size, size_t num_elements);

ctache_data_t
*ctache_data_create_string(char *str, size_t len);

ctache_data_t
*ctache_data_create_hash();

ctache_data_t
*ctache_data_create(enum ctache_data_type data_type,
                    void *data,
                    size_t element_size,
                    size_t num_elements);

void
ctache_data_destroy(void *data);

void
ctache_data_hash_table_set(ctache_data_t *data, const char *key, void *value);

void
*ctache_data_hash_table_get(ctache_data_t *data, const char *key);

void
ctache_data_array_append(ctache_data_t *data, void *value);

void
*ctache_data_array_get(ctache_data_t *data, int index);

size_t
ctache_data_length(ctache_data_t *data);

bool
ctache_data_is_array(ctache_data_t *data);

bool
ctache_data_hash_table_has_key(ctache_data_t *data, const char *key);

#endif /* CTACHE_DATA_H */
