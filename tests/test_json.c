/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2017 David Jackson
 */

#include "json.h"
#include "astrounit.h"
#include <string.h>

ASTRO_TEST_BEGIN(test_empty_object)
{
	char *str = "{}";
	size_t str_len = strlen(str);
	struct json_parser *parser = json_parse_string(str, str_len);
	const struct json_token *tok;
	assert(parser != NULL, "parser creation failed");
	tok = json_next_token(parser);
	assert(tok != NULL, "Token is null");
	assert_int_eq(JSON_BRACE_LEFT, tok->type, "Wrong token");
	tok = json_next_token(parser);
	assert(tok != NULL, "Token is null");
	assert_int_eq(JSON_BRACE_RIGHT, tok->type, "Wrong token");
	tok = json_next_token(parser);
	assert(tok != NULL, "Token is null");
	assert_int_eq(JSON_END, tok->type, "Wrong token, should be JSON_END");
	json_parser_destroy(parser);
}
ASTRO_TEST_END

int
main(void)
{
    int num_failures;
    struct astro_suite *suite;

    suite = astro_suite_create();
    astro_suite_add_test(suite, test_empty_object, NULL);
    astro_suite_run(suite);
    num_failures = astro_suite_num_failures(suite);
    astro_suite_destroy(suite);

    return (num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
