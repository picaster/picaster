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

#ifndef __UI__TRACK_BUTTON_INCLUDED
#define __UI__TRACK_BUTTON_INCLUDED

#include <gtk/gtk.h>

typedef struct {

    gchar*          file_path;
    gchar*          button_name;
    GtkLabel*       label;
    GtkProgressBar* progress_bar;
    gpointer        audio_context;

} TrackButtonData;

void
p_gtk_init_track_buttons();

#endif
