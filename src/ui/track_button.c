/**
    PiCaster is a podcast recording and streaming application,
    easy to use for beginners, but with enough functionality
    to suit the needs of intermediate or experienced podcasters.

    Copyright (C) 2018  Yannick Mauray

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "track_button.h"

#include <stdlib.h>
#include <string.h>
#include <taglib/tag_c.h>
#include <gtk/gtk.h>

#include "../audio/gstreamer.h"
#include "../context.h"

static void
cb_print_position(gint64 pos, gint64 len, gpointer user_data)
{
    GtkButton* button = (GtkButton*)user_data;
    TrackButtonData* track_button_data = (TrackButtonData*)g_object_get_data(G_OBJECT(button), "button_data");
    gtk_progress_bar_set_fraction(track_button_data->progress_bar, (gdouble)pos / (gdouble)len);
    gint64 rem = (len - pos) / 1000000000;
    gint seconds = rem % 60;
    gint minutes = (rem - seconds) / 60;
    gchar* time_remaining = g_strdup_printf("%02d:%02d", minutes, seconds);
    gtk_progress_bar_set_text(track_button_data->progress_bar, time_remaining);
    g_free(time_remaining);
}

void
on_trk_button_clicked(GtkButton* button, gpointer user_data)
{
    if (!context.jack_initialized) return;
    TrackButtonData* track_button_data = (TrackButtonData*)g_object_get_data(G_OBJECT(button), "button_data");
    if (track_button_data->file_path != NULL)
    {
        GtkStyleContext *context;
        context = gtk_widget_get_style_context(GTK_WIDGET(button));

        if (track_button_data->audio_context == NULL)
        {
            track_button_data->audio_context = p_gstreamer_play_track(track_button_data->file_path, cb_print_position, button);
            gtk_style_context_add_class(context,"button_playing");
        }
        else
        {
            p_gstreamer_stop_track(track_button_data->audio_context);
            track_button_data->audio_context = NULL;
            gtk_style_context_remove_class(context,"button_playing");
        }
    }
}

void
p_gtk_init_track_buttons()
{
    for (int i = 1; i <= 8; i++)
    {
        TrackButtonData* track_button_data = (TrackButtonData*)calloc(1, sizeof(TrackButtonData));
        gchar* button_name = g_strdup_printf("trk%d_button", i);
        track_button_data->button_name = g_strdup_printf("TRK%d", i);
        gchar* label_name = g_strdup_printf("%s_label", button_name);
        track_button_data->label = GTK_LABEL(gtk_builder_get_object(context.builder, label_name));
        g_free(label_name);
        gchar* progress_bar_name = g_strdup_printf("%s_progressbar", button_name);
        track_button_data->progress_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(context.builder, progress_bar_name));
        g_free(progress_bar_name);

        GtkButton* button = GTK_BUTTON(gtk_builder_get_object(context.builder, button_name));
        g_object_set_data(G_OBJECT(button), "button_data", track_button_data);
    }

    for (int i = 1; i <= 6; i++)
    {
        TrackButtonData* track_button_data = (TrackButtonData*)calloc(1, sizeof(TrackButtonData));
        gchar* button_name = g_strdup_printf("fx%d_button", i);
        track_button_data->button_name = g_strdup_printf("FX%d", i);
        gchar* label_name = g_strdup_printf("%s_label", button_name);
        track_button_data->label = GTK_LABEL(gtk_builder_get_object(context.builder, label_name));
        g_free(label_name);

        GtkButton* button = GTK_BUTTON(gtk_builder_get_object(context.builder, button_name));
        g_object_set_data(G_OBJECT(button), "button_data", track_button_data);
    }
}

gboolean
on_trk_button_button_release_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GdkEventButton* event_button = (GdkEventButton*)event;
    if (event_button->button == 3)
    {
        TrackButtonData* track_button_data = (TrackButtonData*)g_object_get_data(G_OBJECT(widget), "button_data");
        if (track_button_data->file_path == NULL)
        {
            GtkWidget* file_chooser_dialog = gtk_file_chooser_dialog_new("Open file", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
            gint res = gtk_dialog_run(GTK_DIALOG(file_chooser_dialog));
            if (res == GTK_RESPONSE_ACCEPT)
            {
                GtkFileChooser* chooser = GTK_FILE_CHOOSER(file_chooser_dialog);
                track_button_data->file_path = gtk_file_chooser_get_filename(chooser);
                TagLib_File* taglib_file = taglib_file_new(track_button_data->file_path);
                if (taglib_file == NULL)
                {
                    g_printerr("Error : %s could not be opened\n", track_button_data->file_path);
                }
                else
                {
                    if (!taglib_file_is_valid(taglib_file))
                    {
                        g_printerr("Error : %s is invalid or has no information\n", track_button_data->file_path);
                    }
                    else
                    {
                        const TagLib_AudioProperties* audioproperties = taglib_file_audioproperties(taglib_file);
                        int length = taglib_audioproperties_length(audioproperties);
                        int seconds = length % 60;
                        int minutes = (length - seconds) / 60;
                        gchar* duration = g_strdup_printf("%02d:%02d", minutes, seconds);

                        TagLib_Tag* tag = taglib_file_tag(taglib_file);
                        char* title = taglib_tag_title(tag);
                        char* artist = taglib_tag_artist(tag);
                        gchar* track_info = g_strdup_printf("%s - %s", artist, title);

                        gtk_label_set_text(track_button_data->label, title);
                        gtk_progress_bar_set_text(track_button_data->progress_bar, duration);
                        gtk_progress_bar_set_fraction(track_button_data->progress_bar, 0);

                        g_free(duration);
                        g_free(track_info);
                    }
                    taglib_tag_free_strings();
                    taglib_file_free(taglib_file);
                }
            }
            gtk_widget_destroy(file_chooser_dialog);
        }
        else
        {
            GtkDialog* dialog = GTK_DIALOG(gtk_builder_get_object(context.builder, "yes_no_dialog"));
            GtkLabel* label = GTK_LABEL(gtk_builder_get_object(context.builder, "yes_no_dialog_label"));
            gtk_label_set_text(label, "Are you sure you want to clear this track ?");

            gint response = gtk_dialog_run(dialog);
            gtk_widget_hide(GTK_WIDGET(dialog));
            if (response == GTK_RESPONSE_YES)
            {
                g_free(track_button_data->file_path);
                track_button_data->file_path = NULL;
                gtk_label_set_text(track_button_data->label, track_button_data->button_name);
                gtk_progress_bar_set_text(track_button_data->progress_bar, "00:00");
                gtk_progress_bar_set_fraction(track_button_data->progress_bar, 0);
            }
        }
    }
    return FALSE;
}
