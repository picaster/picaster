bin_PROGRAMS = picaster
picaster_SOURCES = picaster.c ui/ui.c context.c #effect_button.c effects_volume.c filedialog.c flac.c fx_button.c gstreamer.c gtk.c jack.c main.c master_volume.c recorder.c settings.c settings_tab.c track_button.c unlock_button.c
picaster_CFLAGS = -DDATADIR=\"$(datadir)\" -std=gnu99 -Wall -Werror @GTK_CFLAGS@ @ALSA_CFLAGS@ #$(GSTREAMER_CFLAGS) $(GMODULE_CFLAGS) $(TAGLIB_CFLAGS) $(LIBSHOUTIDJC_CFLAGS) $(LIBSNDFILE_CFLAGS)
picaster_LDADD = @GTK_LIBS@ @ALSA_LIBS@ #$(GSTREAMER_LIBS) $(GMODULE_LIBS) $(TAGLIB_LIBS) $(LIBSHOUTIDJC_LIBS) $(LIBSNDFILE_LIBS)
