AC_INIT([picaster], [0.4], [yannick@frenchguy.ch])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])

AC_CHECK_HEADERS([stdio.h])

dnl PKG_CHECK_MODULES([GTK], [gtk+-3.0])
dnl PKG_CHECK_MODULES([ALSA], [alsa])
PKG_CHECK_MODULES([JACK2], [jack > 1.9])
dnl PKG_CHECK_MODULES([GSTREAMER], [gstreamer-1.0])
dnl PKG_CHECK_MODULES([TAGLIB], [taglib_c])
PKG_CHECK_MODULES([SNDFILE], [sndfile])
PKG_CHECK_MODULES([FLAC], [flac])

AC_ARG_WITH([dummy-driver],
  [AS_HELP_STRING([--with-dummy-driver],
    [enable jack's dummy driver])],
  [AC_DEFINE([USE_DUMMY_DRIVER], [1], [Define to use jack's dummy driver])],
  []
)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    libaudio/Makefile
])
AC_OUTPUT