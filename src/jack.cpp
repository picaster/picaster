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

#include "jack.h"
#include "context.h"

#include "AppSettingsManager.h"

extern "C" {

void
on_jackd_path_entry_changed(GtkEntry* entry, gpointer user_data)
{
    const gchar* text = gtk_entry_get_text(entry);
    AppSettingsManager::saveString("jack", "jackd_path_entry", text);
}

void
on_jack_samplerate_combobox_text_changed(GtkComboBox* combo_box, gpointer user_data)
{
    const gchar* text = gtk_combo_box_get_active_id(combo_box);
    AppSettingsManager::saveString("jack", "sample_rate", text);
}

void 
on_start_jack_toggle_button_toggled(GtkToggleButton* toggle_button, gpointer user_data)
{

}

}

void
init_jack_settings()
{
    gchar* jackd_path = AppSettingsManager::getString("jack", "jackd_path_entry", "/usr/bin/jackd");
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(context.builder, "jackd_path_entry")), jackd_path);

    gchar* sample_rate = AppSettingsManager::getString("jack", "sample_rate", "44100");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(gtk_builder_get_object(context.builder, "jack_samplerate_combobox_text")), sample_rate);    
}