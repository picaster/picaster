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
valademo_SOURCES  += main_page/BaseMusicButton.vala
valademo_SOURCES  += main_page/FxButton.vala
valademo_SOURCES  += main_page/TrackButton.vala
valademo_SOURCES  += main_page/MicButton.vala
valademo_SOURCES  += main_page/ButtonBox.vala
valademo_SOURCES  += main_page/FaderBox.vala
valademo_SOURCES  += main_page/MainPage.vala
valademo_SOURCES  += main_page/ControlBox.vala
valademo_SOURCES  += main_page/StartJackButton.vala
valademo_SOURCES  += main_page/StartStreamerButton.vala
valademo_SOURCES  += main_page/StartRecorderButton.vala
valademo_SOURCES  += common/Fader.vala
valademo_SOURCES  += common/FileChooser.vala
valademo_SOURCES  += common/Settings.vala
valademo_SOURCES  += common/Utils.vala

valademo_VALAFLAGS = --pkg gtk+-3.0 --pkg taglib_c

valademo_CPPFLAGS  = @GTK_CFLAGS@
valademo_CPPFLAGS += @GLIB_CFLAGS@
valademo_CPPFLAGS += @TAGLIB_CFLAGS@

valademo_LDADD  = ../libaudio/libaudio.la -lm
valademo_LDADD += @GTK_LIBS@
valademo_LDADD += @GLIB_LIBS@
valademo_LDADD += @TAGLIB_LIBS@