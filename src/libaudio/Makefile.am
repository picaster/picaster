noinst_LTLIBRARIES = libpicaster-audio.la

libpicaster_audio_la_SOURCES  = libpicaster_audio.c libpicaster_audio.h
libpicaster_audio_la_SOURCES += dummy-audio.c dummy-audio.h
libpicaster_audio_la_SOURCES += gstreamer.c gstreamer.h

libpicaster_audio_la_CFLAGS  = -std=gnu99 -Wall -Werror
libpicaster_audio_la_CFLAGS += @GSTREAMER_CFLAGS@
libpicaster_audio_la_CFLAGS += @JACK2_CFLAGS@

libpicaster_audio_la_LIBADD  = @GSTREAMER_LIBS@
libpicaster_audio_la_LIBADD += @JACK2_LIBS@
