/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CTACHE_DATA_H
#define CTACHE_DATA_H

enum ctache_data_type {
    CTACHE_DATA_HASH,
    CTACHE_DATA_ARRAY,
    CTACHE_DATA_STRING,
    CTACHE_DATA_NUMBER,
    CTACHE_DATA_BOOLEAN
};

typedef struct ctache_data ctache_data_t;

ctache_data_t
*ctache_data_create_array(size_t element_size, size_t num_elements);

ctache_data_t
*ctache_data_create_string(char *str, size_t len);

ctache_data_t
*ctache_data_create(enum ctache_data_type data_type,
                    void *data,
                    size_t element_size,
                    size_t num_elements);

void
ctache_data_destroy(void *data);

#endif /* CTACHE_DATA_H */
