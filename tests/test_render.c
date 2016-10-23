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
#include "escaping.h"
#include "astrounit.h"
#include <string.h>
#include <stdlib.h>

ASTRO_TEST_BEGIN(test_render_string_to_string)
{
    char input[] = "The value at 'test' is {{test}}.";
    size_t input_len = strlen(input);
    char correct_output[] = "The value at 'test' is testing.";

    ctache_data_t *data = ctache_data_create_hash();
    ctache_data_t *value_data = ctache_data_create_string("testing", 7);
    ctache_data_hash_table_set(data, "test", value_data);

    char *output;
    ctache_render_string_to_string(input,
                                   input_len,
                                   &output,
                                   data,
                                   ESCAPE_HTML,
                                   "{{",
                                   "}}");

    assert_str_eq(correct_output, output, "rendered string is incorrect");

    free(output);
    ctache_data_destroy(data);
}
ASTRO_TEST_END

int
main(void)
{
    int num_failures;
    struct astro_suite *suite;

    suite = astro_suite_create();
    astro_suite_add_test(suite, test_render_string_to_string, NULL);
    astro_suite_run(suite);
    num_failures = astro_suite_num_failures(suite);
    astro_suite_destroy(suite);

    return (num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}