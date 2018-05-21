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

#include "microphone.h"

#include <gtk/gtk.h>

#include "../context.h"

void
on_microphone_toggle_button_toggled(GtkToggleButton* toggle_button, gpointer user_data)
{
    context.microphone_opened = gtk_toggle_button_get_active(toggle_button);
}

void
on_dj_volume_adjustment_value_changed(GtkAdjustment* adjustment, gpointer user_data)
{
    // volume = pow(linear_volume / 100.0f, 3) * 100.0
    // https://www.dr-lex.be/info-stuff/volumecontrols.html

    gdouble linear_volume = 100.0 - gtk_adjustment_get_value(adjustment);
    context.dj_volume = linear_volume * linear_volume * linear_volume / 100.0f / 100.0f / 100.0f;
}
