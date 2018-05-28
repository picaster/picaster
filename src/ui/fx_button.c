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

#include "fx_button.h"

#include <stdlib.h>

#include "common_button.h"
#include "../context.h"
#include "libaudio/libpicaster_audio.h"

void
p_gtk_init_fx_buttons()
{
    for (int i = 1; i <= 6; i++)
    {
        p_gtk_init_common_button("fx", i);
    }    
}

gboolean
on_fx_button_release_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    p_gtk_common_button_released(widget, event, user_data);
    return FALSE;
}

static gpointer
start_audio_callback(GtkButton* button, const char* file_path)
{
    return p_gstreamer_play_fx(file_path, p_gtk_common_button_update, button);
}

static void
stop_audio_callback(gpointer audio_context)
{
    p_gstreamer_stop_fx(audio_context);
}

void
on_fx_button_clicked(GtkButton* button, gpointer user_data)
{
    p_gtk_common_button_clicked(button, start_audio_callback, stop_audio_callback);
}
