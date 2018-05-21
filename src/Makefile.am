bin_PROGRAMS = picaster
picaster_SOURCES = log.c picaster.c audio/gstreamer.c audio/jack.c ui/device_lists.c ui/jack.c ui/microphone.c ui/mumble.c ui/ui.c context.c settings.c #effect_button.c effects_volume.c filedialog.c flac.c fx_button.c gstreamer.c gtk.c main.c master_volume.c recorder.c settings_tab.c track_button.c unlock_button.c
picaster_CFLAGS = -DDATADIR=\"$(datadir)\" -rdynamic -std=gnu99 -Wall -Werror @GTK_CFLAGS@ @ALSA_CFLAGS@ @JACK2_CFLAGS@ @GSTREAMER_CFLAGS@ #$(GMODULE_CFLAGS) $(TAGLIB_CFLAGS) $(LIBSHOUTIDJC_CFLAGS) $(LIBSNDFILE_CFLAGS)
picaster_LDADD = @GTK_LIBS@ @ALSA_LIBS@ @JACK2_LIBS@ @GSTREAMER_LIBS@ #$(GMODULE_LIBS) $(TAGLIB_LIBS) $(LIBSHOUTIDJC_LIBS) $(LIBSNDFILE_LIBS)
