SUBDIRS = libjack libmumble .

AM_CPPFLAGS = -I$(top_srcdir)/src/libjack
AM_CPPFLAGS += -I$(top_srcdir)/src/libmumble

bin_PROGRAMS = picaster

picaster_SOURCES  = log.c log.h 
picaster_SOURCES += picaster.c picaster.h 
picaster_SOURCES += audio/gstreamer.c audio/gstreamer.h
picaster_SOURCES += audio/jack.c audio/jack.h
picaster_SOURCES += ui/device_lists.c ui/device_lists.h
picaster_SOURCES += ui/jack.c ui/jack.h
picaster_SOURCES += ui/microphone.c ui/microphone.h
picaster_SOURCES += ui/mumble.c ui/mumble.h
picaster_SOURCES += ui/track_button.c ui/track_button.h
picaster_SOURCES += ui/ui.c ui/ui.h
picaster_SOURCES += context.c context.h
picaster_SOURCES += settings.c settings.h

picaster_CFLAGS  = -DDATADIR=\"$(datadir)\" -rdynamic -std=gnu99 -Wall -Werror 
picaster_CFLAGS += @GTK_CFLAGS@
picaster_CFLAGS += @ALSA_CFLAGS@
picaster_CFLAGS += @JACK2_CFLAGS@
picaster_CFLAGS += @GSTREAMER_CFLAGS@
picaster_CFLAGS += @TAGLIB_CFLAGS@

picaster_LDADD  = libjack/libpicaster-jack.la
picaster_LDADD += libmumble/libpicaster-mumble.la
picaster_LDADD += @GTK_LIBS@
picaster_LDADD += @ALSA_LIBS@
picaster_LDADD += @JACK2_LIBS@
picaster_LDADD += @GSTREAMER_LIBS@
picaster_LDADD += @TAGLIB_LIBS@
