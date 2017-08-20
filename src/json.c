/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2017 David Jackson
 */

#include "json.h"
#include "strbuf.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DEFAULT_BUF_SIZE 1024

static char err_unterminated_string[] = "Unterminated string";

static void
parse_string(struct json_parser *parser);

static void
parse_number(struct json_parser *parser);

struct json_parser
*json_parse_file(FILE *fp)
{
	int ch;
	struct strbuf *strbuf;
	struct json_parser *parser;

	strbuf = strbuf_create(DEFAULT_BUF_SIZE);
	if (strbuf == NULL) {
		perror("json_parse_file");
		return NULL;
	}

	while ((ch = fgetc(fp)) != EOF) {
		strbuf_append_char(strbuf, ch);
	}
	strbuf_null_terminate(strbuf);
	parser = malloc(sizeof(struct json_parser));
	if (parser == NULL) {
		perror("json_parse_file");
		strbuf_destroy(strbuf);
		return NULL;
	}
	parser->json = strdup(strbuf_buffer(strbuf));
	parser->json_p = parser->json;
	parser->free_json = true;
	parser->error = JSON_ENOERR;
	strbuf_destroy(strbuf);
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
	int done;

	if (parser->token.type == JSON_STRING) {
		free(parser->token.value.string);
	}
	parser->token.type = JSON_NONE_TOK;
	done = 0;
	while (!done) {
		ch = *(parser->json_p);
		if (ch == '\0') {
			parser->token.type = JSON_END;
			return &(parser->token);
		}
		parser->json_p++;

		if (ch == '{') {
			parser->token.type = JSON_BRACE_LEFT;
			done = 1;
		} else if (ch == '}') {
			parser->token.type = JSON_BRACE_RIGHT;
			done = 1;
		} else if (ch == '[') {
			parser->token.type = JSON_BRACKET_LEFT;
			done = 1;
		} else if (ch == ']') {
			parser->token.type = JSON_BRACKET_RIGHT;
			done = 1;
		} else if (ch == '"') {
			parser->token.type = JSON_STRING;
			parse_string(parser);
			done = 1;
		} else if (ch == ':') {
			parser->token.type = JSON_COLON;
			done = 1;
		} else if (ch == '-' || isdigit(ch)) {
			parser->json_p--;
			parse_number(parser);
			done = 1;
		} else if (isspace(ch)) {
			/* skip spaces */
		} else {
			parser->error = JSON_EBADTOK;
			done = 1;
		}
	}

	return &(parser->token);
}

static void
parse_string(struct json_parser *parser)
{
	char ch;
	struct strbuf *strbuf;

	strbuf = strbuf_create(20);
	if (strbuf == NULL) {
		return;
	}
	while ((ch = *(parser->json_p++)) != '\0' && ch != '"') {
		if (strbuf_append_char(strbuf, ch) < 0) {
			parser->error = JSON_ENOMEM;
			strbuf_destroy(strbuf);
			return;
		}
	}
	strbuf_null_terminate(strbuf);
	if (ch == '"') {
		parser->token.value.string = strdup(strbuf_buffer(strbuf));
	} else {
		strbuf_destroy(strbuf);
		parser->error = JSON_ESYNTAX;
		parser->error_message = err_unterminated_string;
	}
}

static void
parse_number(struct json_parser *parser)
{
	struct strbuf *strbuf;
	char ch;
	double number;

	strbuf = strbuf_create(20);
	while ((ch = *(parser->json_p++)) != '\0' && isdigit(ch)) {
		if (strbuf_append_char(strbuf, ch) < 0) {
			parser->error = JSON_ENOMEM;
			goto cleanup;
		}
	}
	if (ch != '.') {
		strbuf_null_terminate(strbuf);
		sscanf(strbuf_buffer(strbuf), "%lf", &number);
		parser->token.type = JSON_NUMBER;
		parser->token.value.number = number;
		goto cleanup;
	}
	strbuf_append_char(strbuf, '.');
	while ((ch = *(parser->json_p++)) != '\0' && isdigit(ch)) {
		if (strbuf_append_char(strbuf, ch) < 0) {
			parser->error = JSON_ENOMEM;
			goto cleanup;
		}
	}
	strbuf_null_terminate(strbuf);
	sscanf(strbuf_buffer(strbuf), "%lf", &number);
	parser->token.type = JSON_NUMBER;
	parser->token.value.number = number;
cleanup:
	strbuf_destroy(strbuf);
}


bool
json_has_error(struct json_parser *parser)
{
	return parser->error != JSON_ENOERR;
}
