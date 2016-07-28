/* uTest - A microscopic unit testing framework */

#include <stdio.h>
#include <string.h>

int
u_assert(int expr)
{
    return !expr;
}

int
u_assert_str_eq(const char *expected, const char *actual)
{
    if (strcmp(expected, actual) == 0) {
        return 0;
    }
    return 1;
}

int
u_assert_int_eq(int expected, int actual)
{
    if (expected == actual) {
        return 0;
    }
    return 1;
}
