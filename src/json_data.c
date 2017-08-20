/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#include "json_data.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ctache_data_t
*read_object(struct json_parser *parser);

ctache_data_t
*data_from_json(const char *file_name)
{
	ctache_data_t *data;
	FILE *fp;
	struct json_parser *parser;
	struct json_token *tok;

	data = NULL;
	fp = fopen(file_name, "r");
	if (fp == NULL) {
		return NULL;
	}
	parser = json_parse_file(fp);
	tok = json_next_token(parser);
	if (tok->type == JSON_BRACE_LEFT) {
		data = read_object(parser);
	} else {
		fprintf(stderr, "Bad JSON data\n");
		return NULL;
	}
	fclose(fp);

	return data;
}

static ctache_data_t
*read_object(struct json_parser *parser)
{
	ctache_data_t *data;
	struct json_token *tok;
	char *key;
	ctache_data_t *val;
	int done = 0;
	size_t len;

	data = ctache_data_create_hash();

	while (!done) {
		tok = json_next_token(parser);
		key = strdup(tok->value.string);
		tok = json_next_token(parser);
		if (tok->type != JSON_COLON) {
			fprintf(stderr, "ERROR: Expected ':'\n");
			ctache_data_destroy(data);
			free(key);
			return NULL;
		}
		tok = json_next_token(parser);
		if (tok->type == JSON_STRING) {
			len = strlen(tok->value.string);
			val = ctache_data_create_string(tok->value.string, len);
		} else if (tok->type == JSON_BRACE_LEFT) {
			val = read_object(parser);
		} else {
			fprintf(stderr, "ERROR: Expected value\n");
			ctache_data_destroy(data);
			free(key);
			return NULL;
		}
		ctache_data_hash_table_set(data, key, val);
		free(key);

		tok = json_next_token(parser);
		if (tok->type == JSON_BRACE_RIGHT) {
			done = 1;
		} else if (tok->type == JSON_COMMA) {
			done = 0;
		} else {
			fprintf(stderr, "ERROR: Expected '}'\n");
			ctache_data_destroy(data);
			return NULL;
		}
	}
	return data;
}
