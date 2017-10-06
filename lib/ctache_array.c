/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016-2017 David Jackson
 */

#include <stdio.h>
#include <stdlib.h>
#include "ctache_array.h"

#define BUF_SIZE 10

ctache_array_t
*ctache_array_create(size_t element_size, size_t num_elements)
{
    struct ctache_array *array;
    size_t bufsize = num_elements;
    if (num_elements < BUF_SIZE) {
        bufsize = BUF_SIZE;
    }
    size_t array_size = sizeof(struct ctache_array) + bufsize * element_size;
    array = malloc(array_size);
    if (array != NULL) {
        array->bufsize = bufsize;
        array->length = 0;
        array->element_size = element_size;
    }
    return array;
}

void
ctache_array_destroy(void *data)
{
    struct ctache_array *array = (struct ctache_array *)(data);
    free(array);
}

void
*ctache_array_get(ctache_array_t *array, size_t index)
{
    void *data;
    if (index > array->length) {
        return NULL;
    }
    byte *byte_ptr = (array->buffer) + index * array->element_size;
    data = (void *)(byte_ptr);
    return data;
}

void
ctache_array_set(ctache_array_t *array, size_t index, void *data)
{
    byte *byte_ptr = (array->buffer) + index * array->element_size;
    byte *data_ptr = (byte *)(data);
    unsigned int i;
    for (i = 0; i < array->element_size; i++) {
        *byte_ptr = *data_ptr;
        byte_ptr++;
        data_ptr++;
    }
}

void
ctache_array_append(ctache_array_t **array_ptr, void *data)
{
    struct ctache_array *array = *array_ptr;
    if (array->length + 1 >= array->bufsize) {
        size_t bufsize = array->bufsize *= 2;
        size_t bufsize_bytes = bufsize * array->element_size;
        size_t array_size = sizeof(struct ctache_array) + bufsize_bytes;
        *array_ptr = realloc(*array_ptr, array_size);
	if (*array_ptr == NULL) {
		fprintf(stderr, "ctache_array_append: Out of memory");
		exit(EXIT_FAILURE);
	}
        array = *array_ptr;
    }
    ctache_array_set(*array_ptr, array->length, data);
    ((*array_ptr)->length)++;
}

#undef BUF_SIZE
