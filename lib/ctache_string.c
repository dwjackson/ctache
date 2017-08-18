/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2017 Daniel Araujo <contact@daniel-araujo.pt>
 */

#include <stdlib.h>
#include <string.h>
#include "ctache_string.h"

ctache_string_t
*ctache_string_create(char *buf, size_t len)
{
    ctache_string_t *string;

    // We will create a block of memory of the size of the struct plus the
    // length of the string so that we can have the buffer right after the
    // struct. The last member of the struct will be a pointer to that data.
    // We will also reserve an extra character at the end which will hold a
    // terminating NUL character so that the buffer can be treated as a c-style
    // string. Keep in mind that for certain encodings, c-style strings are not
    // appropriate.
    string = malloc(sizeof(struct ctache_string) + len + 1);

    if (string == NULL) {
        return NULL;
    }

    string->length = len;

    memcpy(string->buffer, buf, string->length);
    string->buffer[string->length] = '\0';

    return string;
}

void
ctache_string_destroy(void *data)
{
    struct ctache_string *string = (struct ctache_string *)(data);
    free(string);
}

char
*ctache_string_buffer(ctache_string_t *string)
{
    return string->buffer;
}


size_t
ctache_string_length(ctache_string_t *string)
{
    return string->length;
}

int
ctache_string_compare(ctache_string_t *string1, ctache_string_t *string2)
{
    if (string1->length != string2->length) {
        return string1->length > string2->length ? 1 : -1;
    }

    // At this point we know that both strings have the same length, so there's
    // no difference in reading the value of length from string1 or string2.
    size_t length = string1->length;

    return strncmp(string1->buffer, string2->buffer, length);
}
