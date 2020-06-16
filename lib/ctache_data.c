/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016-2020 David Jackson
 * Modified work Copyright 2017 Daniel Araujo <contact@daniel-araujo.pt>
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "ctache_data.h"
#include "ctache_array.h"
#include "ctache_string.h"

ctache_data_t
*ctache_data_create_array(size_t num_elements)
{
    size_t element_size = sizeof(ctache_data_t);
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
*ctache_data_create_double(double number)
{
    return ctache_data_create(CTACHE_DATA_NUMBER, &number, sizeof(double), 1);
}

ctache_data_t
*ctache_data_create_int(int number)
{
    double d = (double) number;
    return ctache_data_create_double(d);
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
    struct ctache_string *string;
    double *dp;
    bool *bp;
    time_t *tp;

    struct ctache_data *ctache_data = malloc(sizeof(struct ctache_data));
    if (ctache_data) {
        ctache_data->data_type = data_type;
        ctache_data->refcount = 0;
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
            string = ctache_string_create(data, num_elements);
            ctache_data->data.string = string;
            break;
        case CTACHE_DATA_NUMBER:
            dp = (double *)(data);
            ctache_data->data.number = *dp;
            break;
        case CTACHE_DATA_BOOLEAN:
            bp = (bool *)(data);
            ctache_data->data.boolean = *bp;
            break;
        case CTACHE_DATA_TIME:
            tp = (time_t *)data;
            ctache_data->data.time = *tp;
            break;
        default:
            break;
        }
    }
    return ctache_data;
}

/*
 * Note: Do not set free_ctache_data when freeing from an array because the
 * array's cells are copies of pointer data, not the actual malloc'd pointers
 * themselves -- those should already have been freed.
 */
static void
_ctache_data_destroy(void *data, bool free_ctache_data)
{
    struct ctache_data *ctache_data = (struct ctache_data *)(data);
    struct ctache_hash_table *hash_table;
    struct ctache_array *array;
    struct ctache_string *string;
    unsigned int i;

    ctache_data->refcount--;
    if (ctache_data->refcount > 0) {
        return; /* Do not delete if data is still in use somewhere */
    }

    switch (ctache_data->data_type) {
    case CTACHE_DATA_HASH:
        hash_table = ctache_data->data.hash;
        ctache_hash_table_destroy(hash_table, ctache_data_destroy);
        break;
    case CTACHE_DATA_STRING:
        string = ctache_data->data.string;
        ctache_string_destroy(string);
        break;
    case CTACHE_DATA_ARRAY:
        array = ctache_data->data.array;
        for (i = 0; i < array->length; i++) {
            void *ptr = ctache_array_get(array, i);
            _ctache_data_destroy(ptr, false);
        }
        ctache_array_destroy(array);
        break;
    default:
        break;
    }
    if (free_ctache_data) {
        free(ctache_data);
    }
}

void
ctache_data_destroy(void *data)
{
    _ctache_data_destroy(data, true);
}

void
ctache_data_hash_table_set(ctache_data_t *data,
                           const char *key,
                           ctache_data_t *value)
{
    value->refcount++;
    ctache_hash_table_set(data->data.hash, key, value);
}

ctache_data_t
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
ctache_data_array_append(ctache_data_t *data, ctache_data_t *value)
{
    ctache_array_append(&(data->data.array), value);
}

ctache_data_t
*ctache_data_array_get(ctache_data_t *data, int index)
{
    return ctache_array_get(data->data.array, index);
}

size_t
ctache_data_length(ctache_data_t *data)
{
    switch (data->data_type) {
    case CTACHE_DATA_ARRAY:
        return data->data.array->length;

    case CTACHE_DATA_STRING:
        return ctache_string_length(data->data.string);

    default:
        return 0;
    }
}

bool
ctache_data_is_array(ctache_data_t *data)
{
    return (data->data_type == CTACHE_DATA_ARRAY);
}

bool
ctache_data_is_boolean(ctache_data_t *data)
{
    return (data->data_type == CTACHE_DATA_BOOLEAN);
}

bool
ctache_data_is_hash(ctache_data_t *data)
{
    return (data->data_type == CTACHE_DATA_HASH);
}

bool
ctache_data_is_number(ctache_data_t *data)
{
    return (data->data_type == CTACHE_DATA_NUMBER);
}

bool
ctache_data_hash_table_has_key(ctache_data_t *data, const char *key)
{
    return ctache_hash_table_has_key(data->data.hash, key);
}

ctache_data_t
*ctache_data_create_boolean(bool value)
{
    return ctache_data_create(CTACHE_DATA_BOOLEAN, &value, -1, -1);
}

ctache_data_t
*ctache_data_create_time(time_t time)
{
    return ctache_data_create(CTACHE_DATA_TIME, &time, -1, -1);
}

ctache_data_t
*ctache_data_hash_get_keys_as_array(ctache_data_t *data)
{
    ctache_data_t *keys_array;
    struct ctache_hash_table_cell *cell;
    struct linked_list *cells;
    struct linked_list_node *curr;
    struct linked_list *keys_list;
    ctache_data_t *key_data;
    char *key;
    int i;

    keys_list = linked_list_create();

    for (i = 0; i < data->data.hash->bufsize; i++) {
        cells = data->data.hash->cells[i];
        if (cells != NULL) {
            curr = cells->first;
            while (curr != NULL) {
                cell = curr->data;
                linked_list_append(keys_list, cell->key);
                curr = curr->next;
            }
        }
    }

    keys_array = ctache_data_create_array(keys_list->length);
    for (curr = keys_list->first; curr != NULL; curr = curr->next) {
        key = curr->data;
        key_data = ctache_data_create_string(key, strlen(key));
        ctache_data_array_append(keys_array, key_data);
    }

    linked_list_destroy(keys_list);

    return keys_array;
}

ctache_data_t
*ctache_data_merge_hashes(ctache_data_t *first, ctache_data_t *second)
{
    ctache_data_t *merged;
    ctache_data_t *first_keys_array;
    ctache_data_t *second_keys_array;
    size_t length;
    unsigned int i;
    ctache_data_t *key_data;
    char *key;
    ctache_data_t *value_data;

    merged = ctache_data_create_hash();
    first_keys_array = ctache_data_hash_get_keys_as_array(first);
    second_keys_array = ctache_data_hash_get_keys_as_array(second);

    /* Add everything from the first hash */
    length = ctache_data_length(first_keys_array);
    for (i = 0; i < length; i++) {
        key_data = ctache_data_array_get(first_keys_array, i);
        key = ctache_string_buffer(key_data->data.string);
        value_data = ctache_data_hash_table_get(first, key);
        ctache_data_hash_table_set(merged, key, value_data);
    }

    /* Add everything from the second hash */
    length = ctache_data_length(second_keys_array);
    for (i = 0; i < length; i++) {
        key_data = ctache_data_array_get(second_keys_array, i);
        key = ctache_string_buffer(key_data->data.string);
        value_data = ctache_data_hash_table_get(second, key);
        ctache_data_hash_table_set(merged, key, value_data);
    }

    ctache_data_destroy(second_keys_array);
    ctache_data_destroy(first_keys_array);

    return merged;
}

int
ctache_data_strcmp(const ctache_data_t *data1, const ctache_data_t *data2)
{
	return ctache_string_compare(data1->data.string, data2->data.string);
}

void
ctache_array_sort(ctache_data_t *array_data,
		  int (*compar)(const void *p1, const void *p2))
{
	ctache_array_t *array = array_data->data.array;
	qsort(array->buffer, array->length, array->element_size, compar);
}

const char
*ctache_data_string_buffer(ctache_data_t *string_data)
{
	ctache_string_t *string = string_data->data.string;
	return ctache_string_buffer(string);
}

double
ctache_data_as_number(ctache_data_t *data)
{
	return data->data.number;
}
