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

#include "device_lists.h"

#include "../settings.h"

void
on_input_device_combo_box_text_changed(GtkComboBox* combo_box, gpointer user_data)
{
    gchar* active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
    p_settings_save_string("jack", "input_device", active_text);
}

void
on_output_device_combo_box_text_changed(GtkComboBox* combo_box, gpointer user_data)
{
    gchar* active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
    p_settings_save_string("jack", "output_device", active_text);
}