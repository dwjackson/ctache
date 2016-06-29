#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ctache], [0.1.0], [dwjackson@users.noreply.github.com])
AC_CONFIG_SRCDIR([build/CMakeFiles/feature_tests.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset munmap strdup strerror])

AC_CONFIG_FILES([Makefile
                 build/Makefile
                 build/include/Makefile
                 build/lib/Makefile
                 build/man/Makefile
                 build/src/Makefile
                 include/Makefile
                 lib/Makefile
                 man/Makefile
                 src/Makefile])
AC_OUTPUT
