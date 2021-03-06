/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 * Modified work Copyright 2017 Daniel Araujo <contact@daniel-araujo.pt>
 */

#include "ctache_data.h"
#include "astrounit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

ASTRO_TEST_BEGIN(test_hash_get_set)
{
    ctache_data_t *hash = ctache_data_create_hash();

    char value[100];
    char key[] = "test_key";
    strcpy(value, "test_value");
    ctache_data_t *value_data = ctache_data_create_string(value, strlen(value));
    ctache_data_hash_table_set(hash, key, value_data);

    assert(ctache_data_hash_table_has_key(hash, key), "Key not in hash");
    ctache_data_t *check_data;
    check_data = ctache_data_hash_table_get(hash, key);

    assert(check_data != NULL, "check_data is NULL");
    assert_int_eq(check_data->data_type,
                    CTACHE_DATA_STRING,
                    "Value is not a string");
    assert_str_eq("test_value", ctache_data_string_buffer(check_data), "Value is wrong");

    ctache_data_destroy(hash);
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_get_keys)
{
    ctache_data_t *hash = ctache_data_create_hash();

    char value[100];
    char key1[] = "test_key1";
    char key2[] = "test_key2";
    char key3[] = "test_key3";
    ctache_data_t *value_data;

    strcpy(value, "test_value1");
    value_data = ctache_data_create_string(value, strlen(value));
    ctache_data_hash_table_set(hash, key1, value_data);
    strcpy(value, "test_value2");
    value_data = ctache_data_create_string(value, strlen(value));
    ctache_data_hash_table_set(hash, key2, value_data);
    strcpy(value, "test_value3");
    value_data = ctache_data_create_string(value, strlen(value));
    ctache_data_hash_table_set(hash, key3, value_data);
    
    ctache_data_t *keys_array = ctache_data_hash_get_keys_as_array(hash);
    int length = ctache_data_length(keys_array);
    assert_int_eq(3, length, "Wrong number of keys");
    ctache_data_t *key_data;
    key_data = ctache_data_array_get(keys_array, 0);
    assert_str_eq("test_key1", ctache_data_string_buffer(key_data), "Wrong first key");
    key_data = ctache_data_array_get(keys_array, 1);
    assert_str_eq("test_key2", ctache_data_string_buffer(key_data), "Wrong second key");
    key_data = ctache_data_array_get(keys_array, 2);
    assert_str_eq("test_key3", ctache_data_string_buffer(key_data), "Wrong third key");

    ctache_data_destroy(keys_array);
    ctache_data_destroy(hash);
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_hash_merge)
{
    ctache_data_t *value_data;
    char value[100];

    ctache_data_t *hash1 = ctache_data_create_hash();
    strcpy(value, "value1");
    value_data = ctache_data_create_string(value, strlen(value));
    ctache_data_hash_table_set(hash1, "key1", value_data);

    ctache_data_t *hash2 = ctache_data_create_hash();
    strcpy(value, "value2");
    value_data = ctache_data_create_string(value, strlen(value));
    ctache_data_hash_table_set(hash2, "key2", value_data);
    assert_int_eq(1, value_data->refcount, "Wrong reference count");

    ctache_data_t *merged_hash = ctache_data_merge_hashes(hash1, hash2);
    ctache_data_t *keys_array;
    assert_int_eq(2, value_data->refcount, "Wrong reference count");
    keys_array = ctache_data_hash_get_keys_as_array(merged_hash);
    assert_int_eq(2, value_data->refcount, "Wrong reference count");
    int num_keys = ctache_data_length(keys_array);
    assert_int_eq(2, num_keys, "Wrong number of keys in merged hash");
    bool has_key = ctache_data_hash_table_has_key(merged_hash, "key2");
    assert(has_key, "Key missing from merged hash");
    value_data = ctache_data_hash_table_get(merged_hash, "key2");
    assert_str_eq(ctache_data_string_buffer(value_data), "value2", "Wrong value for key2");
    /* merged_hash + hash2 = count = 3 */
    /* Note that the array should not increase the refcount because the data
     * is copied, it is not pointed to. */
    assert_int_eq(2, value_data->refcount, "Wrong reference count");

    assert_int_eq(0, keys_array->refcount, "Keys array has wrong refcount");
    ctache_data_destroy(keys_array);
    assert_int_eq(2, value_data->refcount, "Wrong reference count");
    ctache_data_destroy(merged_hash);
    assert_int_eq(1, value_data->refcount, "Wrong reference count");

    ctache_data_destroy(hash2);
    ctache_data_destroy(hash1);
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_empty_hash)
{
    ctache_data_t *empty_hash = ctache_data_create_hash();
    ctache_data_t *keys_array = ctache_data_hash_get_keys_as_array(empty_hash);
    int num_keys = ctache_data_length(keys_array);
    assert_int_eq(0, num_keys, "Wrong number of keys in \"empty\" hash");
    ctache_data_destroy(keys_array);
    ctache_data_destroy(empty_hash);
}
ASTRO_TEST_END

int
main(void)
{
    int num_failures;
    struct astro_suite *suite;

    suite = astro_suite_create();
    astro_suite_add_test(suite, test_hash_get_set, NULL);
    astro_suite_add_test(suite, test_get_keys, NULL);
    astro_suite_add_test(suite, test_hash_merge, NULL);
    astro_suite_add_test(suite, test_empty_hash, NULL);
    num_failures = astro_suite_run(suite);
    astro_suite_destroy(suite);

    return (num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
