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
	JSON_NONE_TOK,
	JSON_BRACE_LEFT,
	JSON_BRACE_RIGHT,
	JSON_BRACKET_LEFT,
	JSON_BRACKET_RIGHT,
	JSON_STRING,
	JSON_COLON,
	JSON_NUMBER,
	JSON_COMMA,
	JSON_END
};

enum json_error {
	JSON_ENOERR,
	JSON_ENOMEM,
	JSON_EBADTOK,
	JSON_ESYNTAX
};

struct json_token {
	enum json_token_type type;
	union {
		char *string;
		double number;
		bool boolean;
		void *nullvalue;
	} value;
};

struct json_parser {
	struct json_token token;
	bool free_json;
	enum json_error error;
	char *error_message;
	char *json;
	char *json_p;
};

struct json_parser
*json_parse_file(FILE *fp);

struct json_parser
*json_parse_string(char *str, size_t str_len);

void
json_parser_destroy(struct json_parser *parser);

struct json_token
*json_next_token(struct json_parser *parser);

bool
json_has_error(struct json_parser *parser);

#endif /* JSON_H */
