#include "utest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUFSIZE 10

struct utest_harness {
    utest_ret_t (*test)(void*);
    void *args;
};

struct utest_suite {
    int num_failures;
    size_t bufsize;
    size_t length;
    struct utest_harness harnesses[];
};

struct utest_suite
*utest_suite_create()
{
    struct utest_suite *suite;
    size_t size = sizeof(struct utest_suite)
        + DEFAULT_BUFSIZE * sizeof(struct utest_harness);
    suite = malloc(size);
    if (suite != NULL) {
        memset(suite, 0, size);
        suite->bufsize = DEFAULT_BUFSIZE;
    }
    return suite;
}

void
utest_suite_destroy(struct utest_suite *suite)
{
    free(suite);
}

void
utest_suite_add_test(struct utest_suite *suite,
                     utest_ret_t (*test)(void*),
                     void *args)
{
    if (suite->length + 1 >= suite->bufsize) {
        suite->bufsize *= 2;
        size_t size = sizeof(struct utest_suite)
            + sizeof(struct utest_harness) * suite->bufsize;
        suite = realloc(suite, size);
    }
    struct utest_harness harness;
    harness.test = test;
    harness.args = args;
    (suite->harnesses)[suite->length] = harness;
    suite->length++;
}

void
utest_suite_run(struct utest_suite *suite)
{
    int i;
    struct utest_harness *harness;
    int num_tests;
    int num_failures;
    char *message;

    num_tests = 0;
    num_failures = 0;
    for (i = 0; i < suite->length; i++) {
        harness = &(suite->harnesses)[i];
        message = (harness->test)(harness->args);
        if (message != NULL) {
            num_failures++;
            printf("%s\n", message);
        }
        num_tests++;
    }
    if (num_failures == 0) {
        printf("----------------------------------------\n");
        printf(" ALL TESTS PASSED\n");
        printf("----------------------------------------\n");
    } else {
        printf("----------------------------------------\n");
        printf(" %d FAILURES / %d TESTS\n", num_failures, num_tests);
        printf("----------------------------------------\n");
    }
}
