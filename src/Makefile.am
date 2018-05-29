AM_CPPFLAGS = -I$(top_srcdir)/libaudio

bin_PROGRAMS = picaster

picaster_SOURCES = picaster.cpp

picaster_CFLAGS = -DDATADIR=\"$(datadir)\" -rdynamic -std=gnu99 -Wall -Werror 

picaster_LDADD = ../libaudio/libaudio.la