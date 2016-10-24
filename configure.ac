#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([axiom_allocator], [v0.8], [BUG-REPORT-ADDRESS])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_CONFIG_SRCDIR([src/axiom_allocator.c])
AC_CONFIG_HEADERS([config.h])

AM_PROG_AR

LT_INIT()
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

# Debug option
AC_ARG_ENABLE(debug,
            AS_HELP_STRING([--enable-debug], [enable debugging, default: no]),
            [case "${enableval}" in
                yes) debug=true ;;
                no)  debug=false ;;
                *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
            esac],
            [debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

# axiom-evi-nic header file
AXIOM_EVI_NIC_INCLUDE="`pwd`/../axiom-evi-nic/include"
AC_ARG_WITH([axiom-evi-nic],
            AS_HELP_STRING([--with-axiom-evi-nic=dir], [Include directory for axiom-evi-nic]),
            [AXIOM_EVI_NIC_INCLUDE="$withval"])
AC_SUBST([AXIOM_EVI_NIC_INCLUDE])

# axiom-evi-apps header file
AXIOM_EVI_APPS_INCLUDE="`pwd`/../axiom-evi-apps/include"
AC_ARG_WITH([axiom-evi-apps],
            AS_HELP_STRING([--with-axiom-evi-apps=dir], [Include directory for axiom-evi-apps]),
            [AXIOM_EVI_APPS_INCLUDE="$withval"])
AC_SUBST([AXIOM_EVI_APPS_INCLUDE])

# axiom-run-lib
AXIOM_RUN_LIB="`pwd`/../axiom-evi-apps/axiom-run/lib"
AC_ARG_WITH([axiom-run-lib],
            AS_HELP_STRING([--with-axiom-run-lib=dir], [Directory for axiom-run-lib]),
            [AXIOM_RUN_LIB_INCLUDE="$withval"])
AC_SUBST([AXIOM_RUN_LIB])

# evi-allocator
AC_ARG_WITH([evi-allocator],
            AS_HELP_STRING([--with-evi-allocator=dir], [Directory where evi-allocator is installed]),
            [EVI_ALLOCATOR_INCLUDE="$withval"/include]
            [EVI_ALLOCATOR_LIB="$withval"/lib])
AC_SUBST([EVI_ALLOCATOR_INCLUDE])
AC_SUBST([EVI_ALLOCATOR_LIB])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday strrchr])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile])
AC_OUTPUT
