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

#include "mumble.h"

#include "../context.h"
#include "../settings.h"

void
p_gtk_init_mumble_settings()
{
    const gchar* mumble_server = p_settings_get_string("mumble", "server_key", NULL);
    const gchar* mumble_username = p_settings_get_string("mumble", "username", NULL);

    GtkComboBoxText* mumble_server_combo_box_text = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(context.builder, "mumble_server_combo_box_text"));
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(mumble_server_combo_box_text), mumble_server);

    GList* cells = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(mumble_server_combo_box_text));
    GList* first_cell = g_list_first(cells);
    GtkCellRendererText* cell_renderer = GTK_CELL_RENDERER_TEXT(first_cell->data);
    g_object_set(G_OBJECT(cell_renderer), "max-width-chars", 24, NULL);
    g_object_set(G_OBJECT(cell_renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);

    GtkEntry* mumble_username_entry = GTK_ENTRY(gtk_builder_get_object(context.builder, "mumble_username_entry"));
    gtk_entry_set_text(mumble_username_entry, mumble_username);
}

void
on_mumble_server_combo_box_text_changed(GtkComboBoxText* combo_box, gpointer user_data)
{
    const gchar* active_text = gtk_combo_box_get_active_id(GTK_COMBO_BOX(combo_box));
    p_settings_save_string("mumble", "server_key", active_text);
}

void
on_mumble_username_entry_changed(GtkEntry* entry, gpointer user_data)
{
    const gchar* text = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(context.builder, "mumble_username_entry")));
    p_settings_save_string("mumble", "username", text);
}
