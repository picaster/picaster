AM_CPPFLAGS = -I$(top_srcdir)/libaudio

bin_PROGRAMS = picaster

picaster_SOURCES = picaster.cpp

picaster_CXXFLAGS = -DDATADIR=\"$(datadir)\" -rdynamic -std=gnu++11 -Wall -Werror 

picaster_LDADD = ../libaudio/libaudio.la