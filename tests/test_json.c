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
	struct json_parser *parser = json_parse_string(str);
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

ASTRO_TEST_BEGIN(test_empty_array)
{
	char str[] = "[]";
	struct json_parser *parser = json_parse_string(str);
	const struct json_token *tok;
	assert(parser != NULL, "parser creation failed");
	tok = json_next_token(parser);
	assert(tok != NULL, "Token is null");
	assert_int_eq(JSON_BRACKET_LEFT, tok->type, "Wrong token");
	tok = json_next_token(parser);
	assert(tok != NULL, "Token is null");
	assert_int_eq(JSON_BRACKET_RIGHT, tok->type, "Wrong token");
	tok = json_next_token(parser);
	assert(tok != NULL, "Token is null");
	assert_int_eq(JSON_END, tok->type, "Wrong token, should be JSON_END");
	json_parser_destroy(parser);
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_simple_object)
{
	char str[] = "{ \"key\": 42 }";
	struct json_parser *parser = json_parse_string(str);
	const struct json_token *tok;
	assert(parser != NULL, "parser creation failed");
	tok = json_next_token(parser);
	assert_int_eq(JSON_BRACE_LEFT, tok->type, "Wrong token");
	tok = json_next_token(parser);
	assert_int_eq(JSON_STRING, tok->type, "Wrong token");
	assert_str_eq("key", tok->value.string, "Key is wrong");
	tok = json_next_token(parser);
	assert_int_eq(JSON_COLON, tok->type, "Wrong token");
	tok = json_next_token(parser);
	assert_int_eq(JSON_NUMBER, tok->type, "Wrong token");
	assert(tok->value.number == 42.0, "Wrong number");
	tok = json_next_token(parser);
	assert_int_eq(JSON_BRACE_RIGHT, tok->type, "Wrong token");
	json_parser_destroy(parser);
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_number_with_decimal)
{
	char str[] = "{ \"key\": 42.5 }";
	struct json_parser *parser = json_parse_string(str);
	const struct json_token *tok;
	tok = json_next_token(parser);
	tok = json_next_token(parser);
	tok = json_next_token(parser);
	tok = json_next_token(parser);
	assert_int_eq(JSON_NUMBER, tok->type, "Wrong token");
	assert(tok->value.number == 42.5, "Wrong number");
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
    astro_suite_add_test(suite, test_empty_array, NULL);
    astro_suite_add_test(suite, test_simple_object, NULL);
    astro_suite_add_test(suite, test_number_with_decimal, NULL);
    num_failures = astro_suite_run(suite);
    astro_suite_destroy(suite);

    return (num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
