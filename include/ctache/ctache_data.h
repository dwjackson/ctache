/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016-2017 David Jackson
 * Modified work Copyright 2017 Daniel Araujo <contact@daniel-araujo.pt>
 */

#ifndef CTACHE_DATA_H
#define CTACHE_DATA_H

#include <stdbool.h>
#include <time.h>

enum ctache_data_type {
    CTACHE_DATA_HASH,
    CTACHE_DATA_ARRAY,
    CTACHE_DATA_STRING,
    CTACHE_DATA_NUMBER,
    CTACHE_DATA_BOOLEAN,
    CTACHE_DATA_TIME
};

struct ctache_data {
    enum ctache_data_type data_type;
    int refcount;
    union {
        struct ctache_hash_table *hash;
        struct ctache_array *array;
        struct ctache_string *string;
        double number;
        bool boolean;
        time_t time;
    } data;
};

typedef struct ctache_data ctache_data_t;

ctache_data_t
*ctache_data_create_array(size_t num_elements);

ctache_data_t
*ctache_data_create_string(char *str, size_t len);

ctache_data_t
*ctache_data_create_hash();

ctache_data_t
*ctache_data_create_double(double num);

ctache_data_t
*ctache_data_create(enum ctache_data_type data_type,
                    void *data,
                    size_t element_size,
                    size_t num_elements);

void
ctache_data_destroy(void *data);

void
ctache_data_hash_table_set(ctache_data_t *data,
                           const char *key,
                           ctache_data_t *value);

ctache_data_t
*ctache_data_hash_table_get(ctache_data_t *data, const char *key);

void
ctache_data_array_append(ctache_data_t *data, ctache_data_t *value);

ctache_data_t
*ctache_data_array_get(ctache_data_t *data, int index);

size_t
ctache_data_length(ctache_data_t *data);

bool
ctache_data_is_array(ctache_data_t *data);

bool
ctache_data_is_boolean(ctache_data_t *data);

bool
ctache_data_is_hash(ctache_data_t *data);

bool
ctache_data_is_number(ctache_data_t *data);

bool
ctache_data_hash_table_has_key(ctache_data_t *data, const char *key);

ctache_data_t
*ctache_data_create_boolean(bool value);

ctache_data_t
*ctache_data_create_time(time_t time);

ctache_data_t
*ctache_data_hash_get_keys_as_array(ctache_data_t *data);

ctache_data_t
*ctache_data_merge_hashes(ctache_data_t *first, ctache_data_t *second);

int
ctache_data_strcmp(const ctache_data_t *data1, const ctache_data_t *data2);

void
ctache_array_sort(ctache_data_t *array_data,
		  int (*compar)(const void *p1, const void *p2));

const char
*ctache_data_string_buffer(ctache_data_t *data);

double
ctache_data_as_number(ctache_data_t *data);

#endif /* CTACHE_DATA_H */
