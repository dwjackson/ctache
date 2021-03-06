/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2017 David Jackson
 * Modified work Copyright 2017 Daniel Araujo <contact@daniel-araujo.pt>
 */

#include "ctache_data.h"
#include "astrounit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int
compar(const void *p1, const void *p2)
{
	const ctache_data_t *d1 = (const ctache_data_t *)p1;
	const ctache_data_t *d2 = (const ctache_data_t *)p2;
	return ctache_data_strcmp(d1, d2);
}

ASTRO_TEST_BEGIN(test_array_sort)
{
	ctache_data_t *s1 = ctache_data_create_string("str2", 4);
	ctache_data_t *s2 = ctache_data_create_string("str1", 4);
	ctache_data_t *s3 = ctache_data_create_string("str4", 4);
	ctache_data_t *s4 = ctache_data_create_string("str3", 4);
	ctache_data_t *arr = ctache_data_create_array(4);
	ctache_data_array_append(arr, s1);
	ctache_data_array_append(arr, s2);
	ctache_data_array_append(arr, s3);
	ctache_data_array_append(arr, s4);
	ctache_array_sort(arr, compar);
	ctache_data_t *s = ctache_data_array_get(arr, 0);
	assert_str_eq("str1", ctache_data_string_buffer(s), "Wrong data in 1st element");
	s = ctache_data_array_get(arr, 1);
	assert_str_eq("str2", ctache_data_string_buffer(s), "Wrong data in 2nd element");
	s = ctache_data_array_get(arr, 2);
	assert_str_eq("str3", ctache_data_string_buffer(s), "Wrong data in 3rd element");
	s = ctache_data_array_get(arr, 3);
	assert_str_eq("str4", ctache_data_string_buffer(s), "Wrong data in 4th element");
	ctache_data_destroy(arr);
}
ASTRO_TEST_END

int
main(void)
{
    int num_failures;
    struct astro_suite *suite;

    suite = astro_suite_create();
    astro_suite_add_test(suite, test_array_sort, NULL);
    num_failures = astro_suite_run(suite);
    astro_suite_destroy(suite);

    return (num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
