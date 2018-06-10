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

#include "context.h"
#include "faders.h"
#include "AppSettingsManager.h"

void
init_fader(const gchar* fader_name, const gchar* adjustment_name, const gchar* default_value)
{
    gchar* svalue = AppSettingsManager::getString("faders", fader_name, "1");
    gdouble value = g_ascii_strtod(svalue, NULL);
    g_free(svalue);
    GtkAdjustment* adjustment = GTK_ADJUSTMENT(gtk_builder_get_object(context.builder, adjustment_name));
    gtk_adjustment_set_value(adjustment, -value);
}

void
init_faders()
{
    init_fader("dj", "dj_volume_adjustment", "1");
    init_fader("tracks", "track_volume_adjustment", "1");
    init_fader("fx", "fx_volume_adjustment", "1");
    init_fader("master", "master_volume_adjustment", "0.89917899646");
    g_object_set_data(G_OBJECT(gtk_builder_get_object(context.builder, "master_volume_adjustment")), "master", (gpointer)"");
}

void
volume_adjustment_value_changed(GtkAdjustment* adjustment, const gchar* fader_name, JackFaderModule* fader_module)
{
    gdouble value = -gtk_adjustment_get_value(adjustment);
    if (context.jack_client->isConnected())
        fader_module->setSliderValue(value);
    gchar* svalue = g_strdup_printf("%f", value);
    AppSettingsManager::saveString("faders", fader_name, svalue);
    g_free(svalue);
}

extern "C" 
{
    void
    on_dj_volume_adjustment_value_changed(GtkAdjustment* adjustment, gpointer user_data)
    {
        volume_adjustment_value_changed(adjustment, "dj", context.dj_fader);
    }

    void
    on_track_volume_adjustment_value_changed(GtkAdjustment* adjustment, gpointer user_data)
    {
        volume_adjustment_value_changed(adjustment, "tracks", context.decks_fader);
    }

    void
    on_fx_volume_adjustment_value_changed(GtkAdjustment* adjustment, gpointer user_data)
    {
        volume_adjustment_value_changed(adjustment, "fx", context.fx_fader);
    }

    void
    on_master_volume_adjustment_value_changed(GtkAdjustment* adjustment, gpointer user_data)
    {
        volume_adjustment_value_changed(adjustment, "master", context.master_fader);
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
            gtk_adjustment_set_value(adjustment, 0);
        }
        else if (event_button->button == 3)
        {
            gpointer master_flag = g_object_get_data(G_OBJECT(gtk_builder_get_object(context.builder, "master_volume_adjustment")), "master");
            if (master_flag == NULL)
            {
                gtk_adjustment_set_value(adjustment, -1);
            }
            else
            {
                gtk_adjustment_set_value(adjustment, -0.89917899646);
            }
        }
        
        return FALSE;
    }
}