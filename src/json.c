/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2017 David Jackson
 */

#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DEFAULT_BUF_SIZE 1024

struct json_parser
*json_parse_file(FILE *fp)
{
	char *buf;
	size_t buf_size;
	size_t buf_len;
	int ch;
	char *tmp;
	struct json_parser *parser;

	buf_size = DEFAULT_BUF_SIZE;
	buf = malloc(buf_size);
	if (buf == NULL) {
		perror("json_parse_file");
		return NULL;
	}

	buf_len = 0;
	while ((ch = fgetc(fp)) != EOF) {
		if (buf_len + 1 >= buf_size - 1) {
			buf_size *= 2;
			tmp = realloc(buf, buf_size);
			if (tmp == NULL) {
				perror("json_parse_file");
				free(buf);
				return NULL;
			}
			buf = tmp;
		}
		buf[buf_len++] = ch;
	}
	buf[buf_len] = '\0';
	parser = malloc(sizeof(struct json_parser));
	if (parser == NULL) {
		perror("json_parse_file");
		free(buf);
		return NULL;
	}
	parser->json = buf;
	parser->json_p = parser->json;
	parser->free_json = true;
	parser->error = JSON_ENOERR;
	return parser;
}

struct json_parser
*json_parse_string(char *str, size_t str_len)
{
	struct json_parser *parser = malloc(sizeof(struct json_parser));
	if (parser == NULL) {
		perror("json_parse_string");
		return NULL;
	}
	parser->json = str;
	parser->json_p = parser->json;
	parser->free_json = false;
	parser->error = JSON_ENOERR;
	return parser;
}

void
json_parser_destroy(struct json_parser *parser)
{
	if (parser->free_json) {
		free(parser->json);
	}
	free(parser);
}

struct json_token
*json_next_token(struct json_parser *parser)
{
	char ch;

	ch = *(parser->json_p);
	if (ch == '\0') {
		parser->token.type = JSON_END;
		return &(parser->token);
	}
	parser->json_p++;

	if (ch == '{') {
		parser->token.type = JSON_BRACE_LEFT;
	} else if (ch == '}') {
		parser->token.type = JSON_BRACE_RIGHT;
	} else if (ch == '[') {
		parser->token.type = JSON_BRACKET_LEFT;
	} else if (ch == ']') {
		parser->token.type = JSON_BRACKET_RIGHT;
	} else {
		parser->error = JSON_EBADTOK;
	}

	return &(parser->token);
}

bool
json_has_error(struct json_parser *parser)
{
	return parser->error != JSON_ENOERR;
}
