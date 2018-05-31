AM_CPPFLAGS = -I$(top_srcdir)/libaudio

bin_PROGRAMS = picaster

picaster_SOURCES = picaster.cpp

picaster_CXXFLAGS = -DDATADIR=\"$(datadir)\" -rdynamic -std=gnu++14 -Wall -Werror -Wno-error=deprecated-declarations

picaster_LDADD = ../libaudio/libaudio.la