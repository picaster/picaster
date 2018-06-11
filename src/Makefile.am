AM_CPPFLAGS = -I$(top_srcdir)/libaudio

bin_PROGRAMS = picaster libaudiodemo valademo

libaudiodemo_SOURCES = libaudiodemo.cpp

libaudiodemo_CXXFLAGS = -DDATADIR=\"$(datadir)\" -rdynamic -std=gnu++14 -Wall -Werror -Wno-error=deprecated-declarations

libaudiodemo_LDADD = ../libaudio/libaudio.la

picaster_SOURCES  = context.cpp context.h
picaster_SOURCES += device_list.cpp device_list.h
picaster_SOURCES += dialog.cpp dialog.h
picaster_SOURCES += dj.cpp dj.h
picaster_SOURCES += faders.cpp faders.h
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

valademo_SOURCES   = PiCaster.vala
valademo_SOURCES  += BaseMusicButton.vala
valademo_SOURCES  += FxButton.vala
valademo_SOURCES  += TrackButton.vala
valademo_SOURCES  += MicButton.vala
valademo_SOURCES  += Utils.vala
valademo_SOURCES  += ButtonBox.vala
valademo_SOURCES  += Fader.vala
valademo_SOURCES  += FaderBox.vala
valademo_SOURCES  += MainPage.vala
valademo_SOURCES  += ControlBox.vala
valademo_SOURCES  += StartJackButton.vala
valademo_SOURCES  += StartStreamerButton.vala
valademo_SOURCES  += StartRecorderButton.vala

valademo_VALAFLAGS = --pkg gtk+-3.0

valademo_CPPFLAGS  = @GTK_CFLAGS@
valademo_CPPFLAGS += @GLIB_CFLAGS@


valademo_LDADD  = ../libaudio/libaudio.la -lm
valademo_LDADD += @GTK_LIBS@
valademo_LDADD += @GLIB_LIBS@
