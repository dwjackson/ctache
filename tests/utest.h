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

/*****************************************************************************
 * ASSERTS                                                                   *
 *****************************************************************************/

/* Generic assert */
#define u_assert(test, failure_message) do { \
    if (!(test)) {                           \
        return failure_message;              \
    }                                        \
} while (0)

/* Assert that two integers are equal */
#define u_assert_int_eq(expected, actual, failure_message) \
    u_assert((expected) == (actual), failure_message)

/* Assert that two strings are equal */
#define u_assert_str_eq(expected, actual, failure_message) \
    u_assert(strcmp((expected), (actual)) == 0, failure_message)

#endif /* UTEST_H */
