#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([axiom_allocator], [0.8], [BUG-REPORT-ADDRESS])

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
#AXIOM_EVI_NIC_INCLUDE="`pwd`/../axiom-evi-nic/include"
#AC_ARG_WITH([axiom-evi-nic],
#            AS_HELP_STRING([--with-axiom-evi-nic=dir], [Include directory for axiom-evi-nic]),
#            [AXIOM_EVI_NIC_INCLUDE="$withval"])
#AC_SUBST([AXIOM_EVI_NIC_INCLUDE])

# axiom-evi-apps header file
#AXIOM_EVI_APPS_INCLUDE="`pwd`/../axiom-evi-apps/include"
#AC_ARG_WITH([axiom-evi-apps],
#            AS_HELP_STRING([--with-axiom-evi-apps=dir], [Include directory for axiom-evi-apps]),
#            [AXIOM_EVI_APPS_INCLUDE="$withval"])
#AC_SUBST([AXIOM_EVI_APPS_INCLUDE])

# axiom-run-lib
#AXIOM_RUN_LIB="`pwd`/../axiom-evi-apps/axiom-run/lib"
#AC_ARG_WITH([axiom-run-lib],
#            AS_HELP_STRING([--with-axiom-run-lib=dir], [Directory for axiom-run-lib]),
#            [AXIOM_RUN_LIB_INCLUDE="$withval"])
#AC_SUBST([AXIOM_RUN_LIB])

# evi-allocator
#AC_ARG_WITH([evi-allocator],
#            AS_HELP_STRING([--with-evi-allocator=dir], [Directory where evi-allocator is installed]),
#            [EVI_ALLOCATOR_INCLUDE="$withval"/include]
#            [EVI_ALLOCATOR_LIB="$withval"/lib]
#            [EVI_ALLOCATOR_LDS="$withval"/axiom-evi-allocator-lib/xs_map64.lds])
#AC_SUBST([EVI_ALLOCATOR_INCLUDE])
#AC_SUBST([EVI_ALLOCATOR_LIB])
#AC_SUBST([EVI_ALLOCATOR_LDS])

# axiom-dev-mem header file
#AXIOM_MEM_DEV_INC="/usr/include"
#AC_ARG_WITH([axiom-mem-dev],
#           AS_HELP_STRING([--with-axiom-mem-dev=dir], [Include directory for axiom-mem-dev]),
#           [AXIOM_MEM_DEV_INC="$withval"]
#)
#AC_SUBST([AXIOM_MEM_DEV_INC])

# Checks for libraries.

AXIOM_SYSROOT="`pwd`/../output/staging"
AC_ARG_WITH([axiom-sysroot],
            AS_HELP_STRING([--with-axiom-sysroot=dir], [Sysroot directory for AXIOM]),	    
            [AXIOM_SYSROOT="$withval"])
AC_SUBST([AXIOM_SYSROOT])
export PKG_CONFIG_PATH=
AC_SUBST([PKG_CONFIG_PATH])
export PKG_CONFIG_LIBDIR="$AXIOM_SYSROOT/usr/lib/pkgconfig:$AXIOM_SYSROOT/usr/share/pkgconfig"
AC_SUBST([PKG_CONFIG_LIBDIR])
export PKG_CONFIG_SYSROOT_DIR="$AXIOM_SYSROOT"
AC_SUBST([PKG_CONFIG_SYSROOT_DIR])
PKG_PROG_PKG_CONFIG([0.28])

# debug :-)
#echo AXIOM_SYSROOT=${AXIOM_SYSROOT}
#echo PKG_CONFIG_PATH=${PKG_CONFIG_PATH}
#echo PKG_CONFIG_LIBDIR=${PKG_CONFIG_LIBDIR}
#echo PKG_CONFIG_SYSROOT_DIR=${PKG_CONFIG_SYSROOT_DIR}

PKG_CHECK_MODULES([axiom_libraries],[axiom_run_api >= 0.9 axiom_init_api >= 0.9 axiom_user_api >= 0.9])
AC_SUBST([axiom_libraries_CFLAGS])
AC_SUBST([axiom_libraries_LIBS])
PKG_CHECK_MODULES([evidence_lmm],[evi_lmm >= 0.1])
AC_SUBST([evidence_lmm_CFLAGS])
AC_SUBST([evidence_lmm_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday strrchr])

AC_CONFIG_FILES([Makefile
		 axiom_allocator.pc
                 src/Makefile
                 test/Makefile])
AC_OUTPUT
