noinst_LTLIBRARIES = libaudio.la

libaudio_la_SOURCES  = libaudio.cpp libaudio.h
libaudio_la_SOURCES += JackClient.cpp JackClient.h
libaudio_la_SOURCES += JackPorts.cpp JackPorts.h
libaudio_la_SOURCES += JackModule.cpp JackModule.h
libaudio_la_SOURCES += PiCasterJackClient.cpp PiCasterJackClient.h

libaudio_la_CFLAGS  = -std=gnu99 -Wall -Werror
libaudio_la_CFLAGS += @JACK2_CFLAGS@
libaudio_la_CFLAGS += @SNDFILE_CFLAGS@
libaudio_la_CFLAGS += @FLAC_CFLAGS@

libaudio_la_LIBADD  = @JACK2_LIBS@
libaudio_la_LIBADD += @SNDFILE_LIBS@
libaudio_la_LIBADD += @FLAC_LIBS@
libaudio_la_LIBADD += -lpthread