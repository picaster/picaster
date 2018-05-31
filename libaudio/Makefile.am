noinst_LTLIBRARIES = libaudio.la

libaudio_la_SOURCES  = JackClient.cpp JackClient.h
libaudio_la_SOURCES += JackPorts.cpp JackPorts.h
libaudio_la_SOURCES += JackModule.cpp JackModule.h
libaudio_la_SOURCES += JackRecorderModule.cpp JackRecorderModule.h

libaudio_la_CXXFLAGS  = -std=gnu++11 -Wall -Werror
libaudio_la_CXXFLAGS += @JACK2_CFLAGS@
libaudio_la_CXXFLAGS += @SNDFILE_CFLAGS@
libaudio_la_CXXFLAGS += @FLAC_CFLAGS@

libaudio_la_LIBADD  = @JACK2_LIBS@
libaudio_la_LIBADD += @SNDFILE_LIBS@
libaudio_la_LIBADD += @FLAC_LIBS@
libaudio_la_LIBADD += -lpthread