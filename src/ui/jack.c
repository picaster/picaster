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

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <gtk/gtk.h>

#include "../context.h"
#include "../settings.h"
#include "../audio/jack.h"
#include "../audio/gstreamer.h"

void
on_jackd_path_entry_changed(GtkEntry* entry, gpointer user_data)
{
    const gchar* text = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(context.builder, "jackd_path_entry")));
    p_settings_save_string("jack", "jackd_path_entry", text);
}

void
on_jack_samplerate_combobox_text_changed(GtkComboBox* sample_rate_combo_box, gpointer user_data)
{
    const gchar* sample_rate = gtk_combo_box_get_active_id(sample_rate_combo_box);
    p_settings_save_string("jack", "sample_rate", sample_rate);
}

static gboolean
p_jack_client_timeout(gpointer user_data)
{
    p_jack_init_client();
    context.jack_initialized = TRUE;
    p_context_notify(NOTIFICATION_JACK_STARTED);
    return FALSE;
}

void
on_start_jack_toggle_button_toggled(GtkToggleButton* toggle_button, gpointer user_data)
{
    gboolean is_active = gtk_toggle_button_get_active(toggle_button);
    gtk_button_set_label(GTK_BUTTON(toggle_button), is_active ? "Stop jackd" : "Start jackd");
    if (is_active)
    {
        p_jack_start_server();
        g_timeout_add(500, p_jack_client_timeout, NULL);
    }
    else
    {
        p_stop_jack_server();
        p_context_notify(NOTIFICATION_JACK_STOPPED);
    }
}

void
on_master_volume_adjustment_value_changed(GtkAdjustment* adjustment, gpointer user_data)
{
    // volume = pow(linear_volume / 100.0f, 3) * 100.0
    // https://www.dr-lex.be/info-stuff/volumecontrols.html

    gdouble linear_volume = 100.0 - gtk_adjustment_get_value(adjustment);
    context.master_volume = linear_volume * linear_volume * linear_volume / 100.0f / 100.0f / 100.0f;
}

void
on_fx_volume_adjustment_value_changed(GtkAdjustment* adjustment, gpointer user_data)
{
    // volume = pow(linear_volume / 100.0f, 3) * 100.0
    // https://www.dr-lex.be/info-stuff/volumecontrols.html

    gdouble linear_volume = 100.0 - gtk_adjustment_get_value(adjustment);
    context.effects_volume = linear_volume * linear_volume * linear_volume / 100.0f / 100.0f / 100.0f;
}

gboolean
on_volume_label_button_press_event(GtkLabel* label, GdkEvent *event, gpointer user_data)
{
    GtkAdjustment* adjustment = GTK_ADJUSTMENT(user_data);
    GdkEventButton* event_button = (GdkEventButton*)event;
    if (event_button->type != GDK_2BUTTON_PRESS)
    {
        return FALSE;
    }
    if (event_button->button == 1)
    {
        gtk_adjustment_set_value(adjustment, 100.0);
    }
    else if (event_button->button == 3)
    {
        gtk_adjustment_set_value(adjustment, 0.0);
    }
    
    return FALSE;
}
