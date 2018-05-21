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

#ifndef __DEVICE_LISTS_H_INCLUDED
#define __DEVICE_LISTS_H_INCLUDED

#include <gtk/gtk.h>

void
on_input_device_combo_box_text_changed(GtkComboBox* combo_box, gpointer user_data);

void
on_output_device_combo_box_text_changed(GtkComboBox* combo_box, gpointer user_data);

#endif
