/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2017 Daniel Araujo <contact@daniel-araujo.pt>
 */

#ifndef CTACHE_STRING_H
#define CTACHE_STRING_H

/*
 * Data structure that represents a string.
 */
struct ctache_string {
    // Lengh of the string.
    size_t length;

    // Underlying buffer of the string.
    char buffer[0];
};

typedef struct ctache_string ctache_string_t;

/*
 * Creates a ctache string.
 *
 * Takes a pointer to the start of an array of characters and the length of the
 * array.
 *
 * Note that you are not supposed to count the terminating NUL character If the
 * array is a c-style
 *
 * On success it returns a pointer to a ctache_string struct. On failure it
 * returns NULL.
 */
ctache_string_t
*ctache_string_create(char *buf, size_t len);

/*
 * Destroys a ctache string.
 *
 * You must pass a pointer to a ctache_string struct that was returned by a
 * call to ctache_string_create, anything else may crash the program.
 *
 * You may only destroy a ctache string once.
 */
void
ctache_string_destroy(void *data);

/*
 * Returns a pointer to the underlying buffer.
 *
 * You may only dereference up to the length of the string plus one. Call
 * ctache_string_length to find out the length.
 *
 * The extra one in the length is reserved for a NUL character so that the
 * buffer can be used as a c-style string.
 */
char *
ctache_string_buffer(ctache_string_t *string);

/*
 * Returns the length of the string.
 */
size_t
ctache_string_length(ctache_string_t *string);

/*
 * Checks whether two strings are equal.
 *
 * Returns 0 if they are equal, any other value if otherwise.
 */
int
ctache_string_compare(ctache_string_t *string1, ctache_string_t *string2);

#endif /* CTACHE_STRING_H */
