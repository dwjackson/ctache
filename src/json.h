/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2017 David Jackson
 */

#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum json_token_type {
	JSON_BRACE_LEFT,
	JSON_BRACE_RIGHT,
	JSON_BRACKET_LEFT,
	JSON_BRACKET_RIGHT
};

struct json_token {
	enum json_token_type type;
	union {
		const char *string;
		double number;
		bool boolean;
		void *nullvalue;
	} value;
};

struct json_parser {
	struct json_token token;
	bool free_json;
	char *json;
	char *json_p;
};

struct json_parser
*json_parse_file(FILE *fp);

struct json_parser
*json_parse_string(char *str, size_t str_len);

void
json_parser_destroy(struct json_parser *parser);

#endif /* JSON_H */
