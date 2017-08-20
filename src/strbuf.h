/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2017 David Jackson
 */

#ifndef STRBUF_H
#define STRBUF_H

#include <stdlib.h>

struct strbuf {
	char *buf;
	size_t size;
	size_t len;
};

struct strbuf
*strbuf_create(size_t initial_size);

void
strbuf_destroy(struct strbuf *strbuf);

int
strbuf_append_char(struct strbuf *strbuf, int ch);

int
strbuf_null_terminate(struct strbuf *strbuf);

const char
*strbuf_buffer(struct strbuf *strbuf);

#endif /* STRBUF_H */
