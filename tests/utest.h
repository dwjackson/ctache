/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#ifndef UTEST_H
#define UTEST_H

#include <string.h>

typedef char *utest_ret_t;

#define UTEST_BEGIN(test_name)        \
    utest_ret_t test_name(void *args) \
    {                                 \
        do

#define UTEST_END  \
        while (0); \
        return 0;  \
    }

struct utest_suite;

struct utest_suite
*utest_suite_create();

void
utest_suite_destroy(struct utest_suite *suite);

void
utest_suite_add_test(struct utest_suite *suite,
                     utest_ret_t (*test)(void*),
                     void *args);

void
utest_suite_run(struct utest_suite *suite);

int
utest_suite_num_failures(struct utest_suite *suite);


/****************************************************************************
 * ERROR PRINTERS                                                           *
 ****************************************************************************/

void
utest_print_failure_int(int expected,
                        int actual,
                        const char *failure_message,
                        const char *file,
                        int line);

void
utest_print_failure_str(const char *expected,
                        const char *actual,
                        const char *failure_message,
                        const char *file,
                        int line);


/*****************************************************************************
 * ASSERTS                                                                   *
 *****************************************************************************/

/* Generic assert */
#define u_assert(test, failure_message) do { \
    if (!(test)) {                           \
        printf("%s\n", failure_message);     \
        return failure_message;              \
    }                                        \
} while (0)

/* Assert that two integers are equal */
#define u_assert_int_eq(expected, actual, msg)                              \
    if ((expected) != (actual)) {                                           \
        utest_print_failure_int(expected, actual, msg, __FILE__, __LINE__); \
        return msg;                                                         \
    }

/* Assert that two strings are equal */
#define u_assert_str_eq(expected, actual, msg)                              \
    if (strcmp((expected), (actual)) != 0) {                                \
        utest_print_failure_str(expected, actual, msg, __FILE__, __LINE__); \
        return msg;                                                         \
    }

#endif /* UTEST_H */
