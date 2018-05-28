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

#include "common_button.h"

#include <stdlib.h>

#include <taglib/tag_c.h>

#include "../context.h"

void
p_gtk_init_common_button(const char* prefix, int index)
{
    PGtkButtonData* button_data = (PGtkButtonData*)calloc(1, sizeof(PGtkButtonData));
    gchar* button_name = g_strdup_printf("%s%d_button", prefix, index);
    
    gchar* upper_prefix = g_ascii_strup(prefix, -1);
    button_data->button_name = g_strdup_printf("%s%d", upper_prefix, index);
    g_free(upper_prefix);

    gchar* label_name = g_strdup_printf("%s_label", button_name);
    button_data->label = GTK_LABEL(gtk_builder_get_object(context.builder, label_name));
    g_free(label_name);

    gchar* progress_bar_name = g_strdup_printf("%s_progressbar", button_name);
    button_data->progress_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(context.builder, progress_bar_name));
    g_free(progress_bar_name);

    gchar* duration_label_name = g_strdup_printf("%s_duration_label", button_name);
    button_data->duration_label = GTK_LABEL(gtk_builder_get_object(context.builder, duration_label_name));
    g_free(duration_label_name);

    GtkButton* button = GTK_BUTTON(gtk_builder_get_object(context.builder, button_name));
    g_object_set_data(G_OBJECT(button), "button_data", button_data);
}

void
p_gtk_common_button_released(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GdkEventButton* event_button = (GdkEventButton*)event;
    if (event_button->button == 3)
    {
        PGtkButtonData* button_data = (PGtkButtonData*)g_object_get_data(G_OBJECT(widget), "button_data");
        if (button_data->file_path == NULL)
        {
            GtkWindow* main_window = GTK_WINDOW(gtk_builder_get_object(context.builder, "mainwindow"));
            GtkWidget* file_chooser_dialog = gtk_file_chooser_dialog_new("Open file", main_window, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
            GtkFileChooser* chooser = GTK_FILE_CHOOSER(file_chooser_dialog);
            GtkFileFilter* file_filter = gtk_file_filter_new();
            gtk_file_filter_add_mime_type(file_filter, "audio/*");
            gtk_file_chooser_add_filter(chooser, file_filter);
            gint res = gtk_dialog_run(GTK_DIALOG(file_chooser_dialog));
            if (res == GTK_RESPONSE_ACCEPT)
            {
                button_data->file_path = gtk_file_chooser_get_filename(chooser);
                TagLib_File* taglib_file = taglib_file_new(button_data->file_path);
                if (taglib_file == NULL)
                {
                }
                else
                {
                    if (!taglib_file_is_valid(taglib_file))
                    {
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

                        gtk_label_set_text(button_data->label, title);
                        if (button_data->progress_bar != NULL)
                        {
                            gtk_progress_bar_set_text(button_data->progress_bar, duration);
                            gtk_progress_bar_set_fraction(button_data->progress_bar, 0);
                        }
                        if (button_data->duration_label)
                        {
                            gtk_label_set_text(button_data->duration_label, duration);
                        }

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
                g_free(button_data->file_path);
                button_data->file_path = NULL;
                gtk_label_set_text(button_data->label, button_data->button_name);
                if (button_data->progress_bar != NULL)
                {
                    gtk_progress_bar_set_text(button_data->progress_bar, "00:00");
                    gtk_progress_bar_set_fraction(button_data->progress_bar, 0);
                }
                if (button_data->duration_label)
                {
                    gtk_label_set_text(button_data->duration_label, "00:00");
                }
            }
        }
    }
}

void
p_gtk_common_button_clicked(GtkButton* button, PStartAudioCallback start_audio_callback, PStopAudioCallback stop_audio_callback)
{
    if (!context.jack_initialized) return;
    PGtkButtonData* button_data = (PGtkButtonData*)g_object_get_data(G_OBJECT(button), "button_data");

    if (button_data->file_path != NULL)
    {
        GtkStyleContext* style_context = gtk_widget_get_style_context(GTK_WIDGET(button));
        if (button_data->audio_context == NULL)
        {
            button_data->stop_audio_callback = stop_audio_callback;
            button_data->audio_context = start_audio_callback(button, button_data->file_path);
            if (button_data->audio_context != NULL)
            {
                gtk_style_context_add_class(style_context, "button_playing");
            }
        }
        else
        {
            stop_audio_callback(button_data->audio_context);
            button_data->audio_context = NULL;
            gtk_style_context_remove_class(style_context, "button_playing");
        }
    }
}

static gboolean
reset_button_callback(gpointer user_data)
{
    GtkButton* button = (GtkButton*)user_data;
    PGtkButtonData* button_data = (PGtkButtonData*)g_object_get_data(G_OBJECT(button), "button_data");
    PStopAudioCallback stop_audio_callback = button_data->stop_audio_callback;
    stop_audio_callback(button_data->audio_context);
    button_data->audio_context = NULL;
    GtkStyleContext* style_context = gtk_widget_get_style_context(GTK_WIDGET(button));
    gtk_style_context_remove_class(style_context,"button_playing");
    return FALSE;
}

void
p_gtk_common_button_update(gint64 pos, gint64 len, gpointer user_data)
{
    GtkButton* button = (GtkButton*)user_data;
    PGtkButtonData* button_data = (PGtkButtonData*)g_object_get_data(G_OBJECT(button), "button_data");
    gint64 rem = (len - pos) / 1000000000;
    gint seconds = rem % 60;
    gint minutes = (rem - seconds) / 60;
    gchar* time_remaining = g_strdup_printf("%02d:%02d", minutes, seconds);

    if (button_data->progress_bar != NULL)
    {
        gtk_progress_bar_set_fraction(button_data->progress_bar, (gdouble)pos / (gdouble)len);
        gtk_progress_bar_set_text(button_data->progress_bar, time_remaining);
    }
    if (button_data->duration_label != NULL)
    {
        gtk_label_set_text(button_data->duration_label, time_remaining);
    }

    g_free(time_remaining);

    if ((pos == -1) && (len == -1))
    {
        g_timeout_add(100, reset_button_callback, button);
    }
}

