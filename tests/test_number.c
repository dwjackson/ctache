/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2020 David Jackson
 */

#include "ctache_data.h"
#include <astrounit.h>
#include <stdlib.h>

ASTRO_TEST_BEGIN(test_double)
{
	ctache_data_t *num = ctache_data_create_double(123.45);
	assert(ctache_data_is_number(num), "num is not a number");
	double d = ctache_data_as_number(num);
	assert_double_eq(123.45, d, 0.01, "num is wrong number");
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_integer)
{
	ctache_data_t *num = ctache_data_create_int(123);
	assert(ctache_data_is_number(num), "num is not a number");
	int i = ctache_data_as_number(num);
	assert_double_eq(123, i, 0, "num is wrong number");
}
ASTRO_TEST_END

int main()
{
	int num_failures = 0;
	struct astro_suite *suite = astro_suite_create();
	astro_suite_add_test(suite, test_double, NULL);
	astro_suite_add_test(suite, test_integer, NULL);
	num_failures = astro_suite_run(suite);
	astro_suite_destroy(suite);
	return num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
