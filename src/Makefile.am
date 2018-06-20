AM_CPPFLAGS = -I$(top_srcdir)/libaudio

bin_PROGRAMS = picaster

picaster_SOURCES   = PiCaster.vala
picaster_SOURCES  += common/Bus.vala
picaster_SOURCES  += common/Fader.vala
picaster_SOURCES  += common/FileChooser.vala
picaster_SOURCES  += common/Settings.vala
picaster_SOURCES  += common/SettingsPage.vala
picaster_SOURCES  += common/Utils.vala
picaster_SOURCES  += jack_settings_page/JackSettingsPage.vala
picaster_SOURCES  += libaudio/JackClient.vala
picaster_SOURCES  += libaudio/JackFaderModule.vala
picaster_SOURCES  += libaudio/JackFilePlayerModule.vala
picaster_SOURCES  += libaudio/JackModule.vala
picaster_SOURCES  += libaudio/JackPorts.vala
picaster_SOURCES  += libaudio/JackRecorderModule.vala
picaster_SOURCES  += libaudio/JackSineWaveGeneratorModule.vala
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
picaster_SOURCES  += recorder_settings_page/RecorderSettingsPage.vala
#picaster_SOURCES  += cplusplus.cpp

picaster_VALAFLAGS = --target-glib 2.32 --vapidir=$(top_srcdir)/pkg --pkg gtk+-3.0 --pkg taglib_c --pkg posix --pkg jack --pkg alsa --pkg sndfile --pkg ffmpeg

picaster_CPPFLAGS  = @GTK_CFLAGS@
picaster_CPPFLAGS += @GLIB_CFLAGS@
picaster_CPPFLAGS += @TAGLIB_CFLAGS@
picaster_CPPFLAGS += @ALSA_CFLAGS@
picaster_CPPFLAGS += -D_GNU_SOURCE


picaster_LDADD  = ../libaudio/libaudio.la -lm
picaster_LDADD += @GTK_LIBS@
picaster_LDADD += @GLIB_LIBS@
picaster_LDADD += @TAGLIB_LIBS@
picaster_LDADD += @ALSA_LIBS@