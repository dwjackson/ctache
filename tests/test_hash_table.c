#include "ctache_data.h"
#include "utest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
test_hash_get_set()
{
    ctache_data_t *hash = ctache_data_create_hash();

    char value[100];
    char key[] = "test_key";
    strcpy(value, "test_value");
    ctache_data_t *value_data = ctache_data_create_string(value, strlen(value));
    ctache_data_hash_table_set(hash, key, value_data);

    int num_failures = 0;
    num_failures += u_assert(ctache_data_hash_table_has_key(hash, key));

    ctache_data_destroy(hash);

    return num_failures;
}

int
main(void)
{
    int num_failures = 0;
    num_failures += test_hash_get_set();
    printf("----------------------------------------\n");
    printf(" FAILURES: %d\n", num_failures);
    printf("----------------------------------------\n");

    return num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
