#include "json.h"
#include <stdio.h>
#include <stdlib.h>

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
		if (buf_len + 1 < buf_size - 1) {
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
