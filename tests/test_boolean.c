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

UTEST_BEGIN(test_string_as_boolean)
{
    ctache_data_t *data = ctache_data_create_hash();
    ctache_data_t *value = ctache_data_create_string("test", 4);
    ctache_data_hash_table_set(data, "key", value);

    char template_str[] = "value = {{#key}}{{key}}{{/key}}";
    FILE *fp = tmpfile();
    u_assert(fp != NULL, "output file is NULL");

    ctache_render_string(template_str,
                         strlen(template_str),
                         fp,
                         data,
                         ESCAPE_HTML,
                         "{{",
                         "}}");
    fseek(fp, 0, SEEK_SET);
    char output[100];
    memset(output, 0, 100);
    fgets(output, 100, fp);

    char correct_output[] = "value = test";
    u_assert_str_eq(correct_output, output, "Wrong output for string-as-bool");

    fclose(fp);
    ctache_data_destroy(data);
}
UTEST_END

int
main(void)
{
    int num_failures;
    struct utest_suite *suite;

    suite = utest_suite_create();
    utest_suite_add_test(suite, test_string_as_boolean, NULL);
    utest_suite_run(suite);
    num_failures = utest_suite_num_failures(suite);
    utest_suite_destroy(suite);

    return (num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
