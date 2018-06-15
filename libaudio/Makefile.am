noinst_LTLIBRARIES = libaudio.la

libaudio_la_SOURCES  = Float32RingBuffer.cpp Float32RingBuffer.h
libaudio_la_SOURCES += JackClient.cpp JackClient.h
libaudio_la_SOURCES += JackFaderModule.cpp JackFaderModule.h
libaudio_la_SOURCES += JackFilePlayerModule.cpp JackFilePlayerModule.h
libaudio_la_SOURCES += JackModule.cpp JackModule.h
libaudio_la_SOURCES += JackPorts.cpp JackPorts.h
libaudio_la_SOURCES += JackRecorderModule.cpp JackRecorderModule.h
libaudio_la_SOURCES += ShoutcastStreamerModule.cpp ShoutcastStreamerModule.h
libaudio_la_SOURCES += libaudio_c.cpp

libaudio_la_CXXFLAGS  = -std=gnu++14 -Wall -Werror -Wno-error=deprecated-declarations
libaudio_la_CXXFLAGS += @JACK2_CFLAGS@
libaudio_la_CXXFLAGS += @SNDFILE_CFLAGS@
libaudio_la_CXXFLAGS += @FLAC_CFLAGS@
libaudio_la_CXXFLAGS += @LIBAVUTIL_CFLAGS@
libaudio_la_CXXFLAGS += @LIBAVFORMAT_CFLAGS@
libaudio_la_CXXFLAGS += @LIBAVCODEC_CFLAGS@
libaudio_la_CXXFLAGS += @LIBSWRESAMPLE_CFLAGS@

libaudio_la_LIBADD  = -lpthread
libaudio_la_LIBADD += @JACK2_LIBS@
libaudio_la_LIBADD += @SNDFILE_LIBS@
libaudio_la_LIBADD += @FLAC_LIBS@
libaudio_la_LIBADD += @LIBAVUTIL_LIBS@
libaudio_la_LIBADD += @LIBAVFORMAT_LIBS@
libaudio_la_LIBADD += @LIBAVCODEC_LIBS@
libaudio_la_LIBADD += @LIBSWRESAMPLE_LIBS@
