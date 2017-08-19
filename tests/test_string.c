/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright 2017 Daniel Araujo <contact@daniel-araujo.pt>
 */

#include "ctache_data.h"
#include "astrounit.h"
#include <stdlib.h>

ASTRO_TEST_BEGIN(test_string_length_respected)
{
    char input[] = "test";
    size_t input_length = sizeof(input);

    ctache_data_t *test_string = ctache_data_create_string(input, input_length);

    assert_int_eq(input_length, ctache_data_length(test_string), "incorrect string length.");

    ctache_data_destroy(test_string);
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_string_nul_terminator)
{
    char input[] = "testing";
    size_t input_length = 4;

    ctache_data_t *test_string = ctache_data_create_string(input, input_length);

    int terminator = ctache_data_string_buffer(test_string)[input_length];

    assert_int_eq('\0', terminator, "nul terminator does not seem to be present.");

    ctache_data_destroy(test_string);
}
ASTRO_TEST_END

int
main(void)
{
    int num_failures;
    struct astro_suite *suite;

    suite = astro_suite_create();
    astro_suite_add_test(suite, test_string_length_respected, NULL);
    astro_suite_add_test(suite, test_string_nul_terminator, NULL);
    astro_suite_run(suite);
    num_failures = astro_suite_num_failures(suite);
    astro_suite_destroy(suite);

    return (num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
