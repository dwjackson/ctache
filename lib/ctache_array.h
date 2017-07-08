/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016-2017 David Jackson
 */

#ifndef CTACHE_ARRAY_H
#define CTACHE_ARRAY_H

typedef char byte;

struct ctache_array {
    size_t bufsize;
    size_t length;
    size_t element_size;
    byte buffer[];
};
typedef struct ctache_array ctache_array_t;

/*
 * Create a (dynami) ctache array. The element size must be given in bytes.
 */
ctache_array_t
*ctache_array_create(size_t element_size, size_t num_elements);

void
ctache_array_destroy(void *data);

void
*ctache_array_get(ctache_array_t *array, int index);

void
ctache_array_set(ctache_array_t *array, size_t index, void *data);

void
ctache_array_append(ctache_array_t **array_ptr, void *data);

void
ctache_array_sort(ctache_data_t *array_data, int (*compar)(void *p1, void *p2));

#endif /* CTACHE_ARRAY_H */
