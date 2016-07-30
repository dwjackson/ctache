/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#include "ctache_data.h"
#include "utest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

UTEST_BEGIN(test_hash_get_set)
{
    ctache_data_t *hash = ctache_data_create_hash();

    char value[100];
    char key[] = "test_key";
    strcpy(value, "test_value");
    ctache_data_t *value_data = ctache_data_create_string(value, strlen(value));
    ctache_data_hash_table_set(hash, key, value_data);

    u_assert(ctache_data_hash_table_has_key(hash, key), "Key not in hash");
    ctache_data_t *check_data;
    check_data = ctache_data_hash_table_get(hash, key);

    u_assert(check_data != NULL, "check_data is NULL");
    u_assert_int_eq(check_data->data_type,
                    CTACHE_DATA_STRING,
                    "Value is not a string");
    u_assert_str_eq("test_value", check_data->data.string, "Value is wrong");

    ctache_data_destroy(hash);
}
UTEST_END

UTEST_BEGIN(test_get_keys)
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
    u_assert_int_eq(3, length, "Wrong number of keys");
    ctache_data_t *key_data;
    key_data = ctache_data_array_get(keys_array, 0);
    u_assert_str_eq("test_key1", key_data->data.string, "Wrong first key");
    u_assert_int_eq(2, key_data->refcount, "Wrong number of references");
    key_data = ctache_data_array_get(keys_array, 1);
    u_assert_str_eq("test_key2", key_data->data.string, "Wrong second key");
    key_data = ctache_data_array_get(keys_array, 2);
    u_assert_str_eq("test_key3", key_data->data.string, "Wrong third key");

    ctache_data_destroy(keys_array);
    ctache_data_destroy(hash);
}
UTEST_END

int
main(void)
{
    int num_failures;
    struct utest_suite *suite;

    suite = utest_suite_create();
    utest_suite_add_test(suite, test_hash_get_set, NULL);
    utest_suite_add_test(suite, test_get_keys, NULL);
    utest_suite_run(suite);
    num_failures = utest_suite_num_failures(suite);
    utest_suite_destroy(suite);

    return (num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
