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

#ifndef __UI__COMMON_BUTTON_H_INCLUDED
#define __UI__COMMON_BUTTON_H_INCLUDED

#include <gtk/gtk.h>

typedef gpointer (*PStartAudioCallback)(GtkButton* button, const char* file_path);
typedef void (*PStopAudioCallback)(gpointer audio_context);

typedef struct {

    gchar*             file_path;
    gchar*             button_name;
    GtkLabel*          label;
    GtkProgressBar*    progress_bar;
    GtkLabel*          duration_label;
    gpointer           audio_context;
    PStopAudioCallback stop_audio_callback;

} PGtkButtonData;

void
p_gtk_init_common_button(const char* prefix, int index);

void
p_gtk_common_button_released(GtkWidget *widget, GdkEvent *event, gpointer user_data);

void
p_gtk_common_button_clicked(GtkButton* button, PStartAudioCallback start_audio_callback, PStopAudioCallback stop_audio_callback);

void
p_gtk_common_button_update(gint64 pos, gint64 len, gpointer user_data);

#endif
