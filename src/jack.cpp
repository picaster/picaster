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
        gboolean is_active = gtk_toggle_button_get_active(toggle_button);
        gtk_button_set_label(GTK_BUTTON(toggle_button), is_active ? "Stop jackd" : "Start jackd");
        if (is_active)
        {
            context.jack_client->setJackdPath(AppSettingsManager::getString("jack", "jackd_path_entry", "/usr/bin/jackd"));
            context.jack_client->setDriver("alsa");
            context.jack_client->setPeriodsPerBuffer("3");
            context.jack_client->setInputDevice(AppSettingsManager::getString("jack", "input_device_id", NULL));
            context.jack_client->setOutputDevice(AppSettingsManager::getString("jack", "output_device_id", NULL));
            context.jack_client->setFramesPerPeriod("256");
            context.jack_client->setSampleRate(AppSettingsManager::getString("jack", "sample_rate", "48000"));
            context.jack_client->startJack([]() {
                init_jack_modules();
            });
        }
        else
        {
            context.jack_client->stopJack();
        }
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

void
init_jack_modules()
{
    if (context.recorder != NULL) return;

    context.recorder = new JackRecorderModule("recorder", context.jack_client);
    context.recorder_fader = new JackFaderModule("recorder_fader", context.jack_client);
    context.streamer = new ShoutcastStreamerModule("streamer", context.jack_client);
    context.master_fader = new JackFaderModule("master_fader", context.jack_client);
    context.dj_fader = new JackFaderModule("dj_fader", context.jack_client);
    context.deck_a = new JackFilePlayerModule("deck_a", context.jack_client);
    context.deck_b = new JackFilePlayerModule("deck_b", context.jack_client);
    context.decks_fader = new JackFaderModule("decks_fader", context.jack_client);
    context.fx = new JackFilePlayerModule("fx", context.jack_client);
    context.fx_fader = new JackFaderModule("fx_fader", context.jack_client);

    JackPorts* playback_ports = context.jack_client->getPlaybackPorts();
    JackPorts* capture_ports = context.jack_client->getCapturePorts();

    context.master_fader->setSliderValue(0.89917899646); /* -3 dBFS */
    context.master_fader->connectTo(playback_ports);
    context.master_fader->connectTo(context.recorder_fader);
    context.recorder_fader->setSliderValue(1); /* 0 dBFS */
    context.recorder_fader->connectTo(context.recorder);
    context.recorder_fader->connectTo(context.streamer);

    capture_ports->connectTo(context.dj_fader);
    context.dj_fader->enable(false);
    context.dj_fader->connectTo(context.master_fader);

    context.deck_a->connectTo(context.decks_fader);
    context.deck_b->connectTo(context.decks_fader);
    context.decks_fader->connectTo(context.master_fader);

    context.fx->connectTo(context.fx_fader);
    context.fx_fader->connectTo(context.master_fader);
}