dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(EvSip, 1.0.0)

AC_CONFIG_SRCDIR([src/evsip_main.c])
AC_CONFIG_HEADERS([src/config.h])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()
AM_SILENT_RULES

# Checks for programs
AC_PROG_CC

# Doxygen support
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_INIT_DOXYGEN($PACKAGE_NAME, Doxyfile, docs)

# Checks for libraries.
PKG_CHECK_MODULES([SOFIA], [sofia-sip-ua])
PKG_CHECK_MODULES([CHECK], [check])

# Checks for header files.
AC_STDC_HEADERS
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h syslog.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([memset])


AC_CONFIG_FILES(Makefile src/Makefile tests/Makefile)
AC_OUTPUT

