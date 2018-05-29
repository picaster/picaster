noinst_LTLIBRARIES = libaudio.la

libaudio_la_SOURCES  = libaudio.cpp libaudio.h
libaudio_la_SOURCES += JackClient.cpp JackClient.h
libaudio_la_SOURCES += JackPorts.cpp JackPorts.h
libaudio_la_SOURCES += JackModule.cpp JackModule.h

libaudio_la_CFLAGS  = -std=gnu99 -Wall -Werror
libaudio_la_CFLAGS += @JACK2_CFLAGS@

libaudio_la_LIBADD  = @JACK2_LIBS@