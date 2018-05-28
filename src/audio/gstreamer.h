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

#ifndef __AUDIO__GSTREAMER_H_INCLUDED
#define __AUDIO__GSTREAMER_H_INCLUDED

#include <gst/gst.h>

typedef void(*P_CALLBACK)(gint64 pos, gint64 len, gpointer user_data);

void
p_gstreamer_init(int* argc, char*** argv);

GstElement*
p_gstreamer_play_track(const gchar* file_path, P_CALLBACK callback, gpointer callback_user_data);

void
p_gstreamer_stop_track(gpointer audio_context);

#endif
