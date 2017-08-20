/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2017 David Jackson
 */

#include "strbuf.h"

struct strbuf
*strbuf_create(size_t initial_size)
{
	struct strbuf *strbuf;

	if (initial_size == 0) {
		return NULL;
	}

	strbuf = malloc(sizeof(struct strbuf));
	if (strbuf == NULL) {
		return NULL;
	}
	strbuf->buf = malloc(initial_size);
	if (strbuf->buf == NULL) {
		free(strbuf);
		return NULL;
	}
	strbuf->size = initial_size;
	strbuf->len = 0;
	return strbuf;
}

void
strbuf_destroy(struct strbuf *strbuf)
{
	free(strbuf->buf);
	free(strbuf);
}

int
strbuf_append_char(struct strbuf *strbuf, int ch)
{
	char *tmp;
	size_t larger_size;

	if (strbuf->len + 1 >= strbuf->size - 1) {
		larger_size = strbuf->size * 2;
		tmp = realloc(strbuf->buf, larger_size);
		if (tmp == NULL) {
			return -1;
		}
		strbuf->size = larger_size;
		strbuf->buf = tmp;
	}
	strbuf->buf[strbuf->len++] = ch;
	return 0;
}

int
strbuf_null_terminate(struct strbuf *strbuf)
{
	return strbuf_append_char(strbuf, '\0');
}

const char
*strbuf_buffer(struct strbuf *strbuf)
{
	return strbuf->buf;
}
