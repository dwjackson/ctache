#ifndef UTEST_H
#define UTEST_H

int
u_assert(int expr);

int
u_assert_str_eq(const char *expected, const char *actual);

int
u_assert_int_eq(int expected, int actual);

#endif /* UTEST_H */
