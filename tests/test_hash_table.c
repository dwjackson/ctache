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

    //u_assert(ctache_data_hash_table_has_key(hash, key), "Key not in hash");
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

int
main(void)
{
    struct utest_suite *suite = utest_suite_create();
    utest_suite_add_test(suite, test_hash_get_set, NULL);
    utest_suite_run(suite);
    utest_suite_destroy(suite);

    return 0;
}
