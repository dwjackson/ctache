/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "hash_table.h"
#include "ctache_data.h"
#include "ctache_array.h"

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
            // TODO
            break;
        }
    }
    return ctache_data;
}

void
ctache_data_destroy(void *data)
{
    struct ctache_data *ctache_data = (struct ctache_data *)(data);
    switch (ctache_data->data_type) { // TODO
    default:
        break;
    }
    // TODO
}
