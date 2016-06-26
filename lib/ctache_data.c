/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "hash_table.h"
#include "ctache_data.h"
#include "ctache_array.h"

ctache_data_t
*ctache_data_create_array(size_t element_size, size_t num_elements)
{
    return ctache_data_create(CTACHE_DATA_ARRAY,
                              NULL,
                              element_size,
                              num_elements);
}

ctache_data_t
*ctache_data_create_string(char *str, size_t len)
{
    return ctache_data_create(CTACHE_DATA_STRING, str, 1, len);
}

ctache_data_t
*ctache_data_create_hash()
{
    struct ctache_hash_table *tbl = ctache_hash_table_create();
    return ctache_data_create(CTACHE_DATA_HASH, tbl, -1, -1);
}

ctache_data_t
*ctache_data_create(enum ctache_data_type data_type,
                    void *data,
                    size_t element_size,
                    size_t num_elements)

{
    struct ctache_hash_table *tbl;
    struct ctache_array *array;
    double *dp;
    bool *bp;
    char *str;

    struct ctache_data *ctache_data = malloc(sizeof(struct ctache_data));
    if (ctache_data) {
        ctache_data->data_type = data_type;
        switch (data_type) {
        case CTACHE_DATA_HASH:
            tbl = (struct ctache_hash_table *)(data);
            ctache_data->data.hash = tbl;
            break;
        case CTACHE_DATA_ARRAY:
            array = ctache_array_create(element_size, num_elements);
            ctache_data->data.array = array;
            break;
        case CTACHE_DATA_STRING:
            str = (char *)(data);
            ctache_data->data.string = str;
            break;
        case CTACHE_DATA_NUMBER:
            dp = (double *)(data);
            ctache_data->data.number = *dp;
            break;
        case CTACHE_DATA_BOOLEAN:
            bp = (bool *)(data);
            ctache_data->data.boolean = *bp;
            break;
        default:
            break;
        }
    }
    return ctache_data;
}

void
ctache_data_destroy(void *data)
{
    struct ctache_data *ctache_data = (struct ctache_data *)(data);
    struct ctache_hash_table *hash_table;
    struct ctache_array *array;
    char *str;

    switch (ctache_data->data_type) { // TODO
    case CTACHE_DATA_HASH:
        hash_table = ctache_data->data.hash;
        ctache_hash_table_destroy(hash_table, ctache_data_destroy);
        break;
    case CTACHE_DATA_STRING:
        str = ctache_data->data.string;
        free(str);
        break;
    case CTACHE_DATA_ARRAY:
        array = ctache_data->data.array;
        // TODO: Free the array cells
        ctache_array_destroy(array);
        break;
    default:
        break;
    }
    free(ctache_data);
}

void
ctache_data_hash_table_set(ctache_data_t *data, const char *key, void *value)
{
    ctache_hash_table_set(data->data.hash, key, value);
}

void
*ctache_data_hash_table_get(ctache_data_t *data, const char *key)
{
    enum ctache_data_type data_type = data->data_type;
    if (data_type != CTACHE_DATA_HASH) {
        fprintf(stderr, "ctache_data_t has wrong data type: %d\n", data_type);
        abort();
    }
    return ctache_hash_table_get(data->data.hash, key);
}

void
ctache_data_array_append(ctache_data_t *data, void *value)
{
    ctache_array_append(&(data->data.array), value);
}

void
*ctache_data_array_get(ctache_data_t *data, int index)
{
    return ctache_array_get(data->data.array, index);
}

size_t
ctache_data_length(ctache_data_t *data)
{
    size_t len = 0;
    if (data->data_type == CTACHE_DATA_ARRAY) {
        len = data->data.array->length;
    }
    return len;
}

bool
ctache_data_is_array(ctache_data_t *data)
{
    return (data->data_type == CTACHE_DATA_ARRAY);
}

bool
ctache_data_hash_table_has_key(ctache_data_t *data, const char *key)
{
    return ctache_hash_table_has_key(data->data.hash, key);
}
