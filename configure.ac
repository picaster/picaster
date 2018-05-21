AC_INIT([picaster], [0.3], [yannick@frenchguy.ch])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC

PKG_CHECK_MODULES([GTK], [gtk+-3.0])
PKG_CHECK_MODULES([ALSA], [alsa])
PKG_CHECK_MODULES([JACK2], [jack > 1.9])
PKG_CHECK_MODULES([GSTREAMER], [gstreamer-1.0])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_OUTPUT
