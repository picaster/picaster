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

#include <unistd.h>
#include <iostream>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "context.h"
#include "ui.h"

int
main(int argc, char** argv)
{
    context.jack_client = JackClient::getInstance("PiCaster");

    if (context.jack_client->isConnected())
    {
        context.recorder = new JackRecorderModule("recorder", context.jack_client);
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

        context.master_fader->setSliderValue(1.0); /* 0 dBFS */
        context.master_fader->connectTo(playback_ports);
        context.master_fader->connectTo(context.recorder);
        context.master_fader->connectTo(context.streamer);

        capture_ports->connectTo(context.dj_fader);
        context.dj_fader->enable(false);
        context.dj_fader->connectTo(context.master_fader);

        context.deck_a->connectTo(context.decks_fader);
        context.deck_b->connectTo(context.decks_fader);
        context.decks_fader->connectTo(context.master_fader);

        context.fx->connectTo(context.fx_fader);
        context.fx_fader->connectTo(context.master_fader);
    }

    /* Init GTK */
    gtk_init(&argc, &argv);

    /* Create application */
    GtkApplication* app = gtk_application_new("ch.frenchguy.picaster", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    /* Run application */
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    return status;
}
