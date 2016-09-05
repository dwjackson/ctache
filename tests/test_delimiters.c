/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#include "ctache_data.h"
#include "render.h"
#include "utest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

UTEST_BEGIN(test_custom_delimiters)
{
    ctache_data_t *data = ctache_data_create_hash();

    char value[100];
    char key[] = "test_key";
    strcpy(value, "test_value");
    ctache_data_t *value_data = ctache_data_create_string(value, strlen(value));
    ctache_data_hash_table_set(data, key, value_data);

    char template[] = "[[test_key]]";
    char out_file_name[] = "delimiter_test_file.txt";
    FILE *fp = fopen(out_file_name, "w");
    if (fp != NULL) {
        ctache_render_string(template,
                             strlen(template),
                             fp,
                             data,
                             ESCAPE_HTML,
                             "[[",
                             "]]");
        fclose(fp);
    }
    char str[100];
    fp = fopen(out_file_name, "r");
    if (fp != NULL) {
        char *s = fgets(str, 100, fp);
        u_assert(s != NULL, "fgets() failed");
        fclose(fp);
        u_assert_str_eq(str, "test_value", "Wrong value in data hash");
    }
}
UTEST_END

int
main(void)
{
    int num_failures;
    struct utest_suite *suite;

    suite = utest_suite_create();
    utest_suite_add_test(suite, test_custom_delimiters, NULL);
    utest_suite_run(suite);
    num_failures = utest_suite_num_failures(suite);
    utest_suite_destroy(suite);

    return (num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
