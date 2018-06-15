AM_CPPFLAGS = -I$(top_srcdir)/libaudio

bin_PROGRAMS = picaster

picaster_SOURCES   = PiCaster.vala
picaster_SOURCES  += main_page/BaseMusicButton.vala
picaster_SOURCES  += main_page/FxButton.vala
picaster_SOURCES  += main_page/TrackButton.vala
picaster_SOURCES  += main_page/MicButton.vala
picaster_SOURCES  += main_page/ButtonBox.vala
picaster_SOURCES  += main_page/FaderBox.vala
picaster_SOURCES  += main_page/MainPage.vala
picaster_SOURCES  += main_page/ControlBox.vala
picaster_SOURCES  += main_page/StartJackButton.vala
picaster_SOURCES  += main_page/StartStreamerButton.vala
picaster_SOURCES  += main_page/StartRecorderButton.vala
picaster_SOURCES  += common/Bus.vala
picaster_SOURCES  += common/Fader.vala
picaster_SOURCES  += common/FileChooser.vala
picaster_SOURCES  += common/Settings.vala
picaster_SOURCES  += common/Utils.vala
picaster_SOURCES  += cplusplus.cpp

picaster_VALAFLAGS = --pkg gtk+-3.0 --pkg taglib_c

picaster_CPPFLAGS  = @GTK_CFLAGS@
picaster_CPPFLAGS += @GLIB_CFLAGS@
picaster_CPPFLAGS += @TAGLIB_CFLAGS@

picaster_LDADD  = ../libaudio/libaudio.la -lm
picaster_LDADD += @GTK_LIBS@
picaster_LDADD += @GLIB_LIBS@
picaster_LDADD += @TAGLIB_LIBS@