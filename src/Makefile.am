AM_CPPFLAGS = -I$(top_srcdir)/libaudio

bin_PROGRAMS = picaster libaudiodemo

libaudiodemo_SOURCES = libaudiodemo.cpp

libaudiodemo_CXXFLAGS = -DDATADIR=\"$(datadir)\" -rdynamic -std=gnu++14 -Wall -Werror -Wno-error=deprecated-declarations

libaudiodemo_LDADD = ../libaudio/libaudio.la

picaster_SOURCES  = context.cpp context.h
picaster_SOURCES += device_list.cpp device_list.h
picaster_SOURCES += dialog.cpp dialog.h
picaster_SOURCES += jack.cpp jack.h
picaster_SOURCES += picaster.cpp
picaster_SOURCES += track_fx_button.cpp track_fx_button.h
picaster_SOURCES += ui.cpp ui.h
picaster_SOURCES += AppSettingsManager.cpp AppSettingsManager.h

picaster_CXXFLAGS  = -DDATADIR=\"$(datadir)\" -rdynamic -std=gnu++14 -Wall -Werror -Wno-error=deprecated-declarations
picaster_CXXFLAGS += @ALSA_CFLAGS@
picaster_CXXFLAGS += @GTK_CFLAGS@
picaster_CXXFLAGS += @TAGLIB_CFLAGS@

picaster_LDADD  = ../libaudio/libaudio.la
picaster_LDADD += @ALSA_LIBS@
picaster_LDADD += @GTK_LIBS@
picaster_LDADD += @TAGLIB_LIBS@
