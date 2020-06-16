#include "ctache_data.h"
#include <astrounit.h>
#include <stdlib.h>

ASTRO_TEST_BEGIN(test_number)
{
	ctache_data_t *num = ctache_data_create_double(123.45);
	assert(ctache_data_is_number(num), "num is not a number");
	double d = ctache_data_as_number(num);
	assert_double_eq(123.45, d, 0.01, "num is wrong number");
}
ASTRO_TEST_END

int main()
{
	int num_failures = 0;
	struct astro_suite *suite = astro_suite_create();
	astro_suite_add_test(suite, test_number, NULL);
	num_failures = astro_suite_run(suite);
	astro_suite_destroy(suite);
	return num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
