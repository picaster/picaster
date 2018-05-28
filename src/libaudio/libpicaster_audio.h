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

#ifndef __LIBPICASTER_AUDIO_H_INCLUDED
#define __LIBPICASTER_AUDIO_H_INCLUDED

#include <gst/gst.h>

typedef void (*PProgressCallback)(gint64 pos, gint64 len, gpointer user_data);

void
libpicaster_audio_init(int* p_argc, char*** p_argv);

gpointer
p_gstreamer_play_fx(const char* file_path, PProgressCallback progress_callback, gpointer callback_user_data);

void
p_gstreamer_stop_fx(gpointer audio_context);

#endif
