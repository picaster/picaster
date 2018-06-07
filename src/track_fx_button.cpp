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

#include <gtk/gtk.h>
#include <taglib/tag_c.h>

#include "context.h"
#include "track_fx_button.h"

static void
on_button_released_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GdkEventButton* event_button = (GdkEventButton*)event;
    if (event_button->button == 3)
    {
        ButtonData* button_data = (ButtonData*)g_object_get_data(G_OBJECT(widget), "button_data");
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

extern "C" {

gboolean
on_trk_button_release_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    on_button_released_event(widget, event, user_data);
    return FALSE;
}

gboolean
on_fx_button_release_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    on_button_released_event(widget, event, user_data);
    return FALSE;
}

}

static void
init_button(const gchar* prefix, int index)
{
    ButtonData* button_data = new ButtonData();
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
init_track_and_fx_buttons()
{
    for (int i = 1; i <= 8; i++)
    {
        init_button("trk", i);
    }

    for (int i = 1; i <= 6; i++)
    {
        init_button("fx", i);
    }
}