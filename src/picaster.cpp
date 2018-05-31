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

#include "JackClient.h"
#include "JackRecorderModule.h"
#include "JackFaderModule.h"

int
main(int argc, char** argv)
{
    JackClient* jack_client = new JackClient();
    jack_client->activate();

    JackRecorderModule* recorder = new JackRecorderModule("recorder", jack_client);

    JackFaderModule* master_fader = new JackFaderModule("master_fader", jack_client);

    JackPorts* playback_ports = jack_client->getPlaybackPorts();
    master_fader->connectTo(playback_ports);
    master_fader->connectTo(recorder);
    recorder->activate();
    master_fader->activate();

    JackPorts* capture_ports = jack_client->getCapturePorts();
    JackModule* dj_fader = new JackFaderModule("dj_fader", jack_client);
    capture_ports->connectTo(dj_fader);
    dj_fader->connectTo(master_fader);
    dj_fader->activate();

    JackPorts* deck_a = jack_client->createOutputPorts("deck_a");
    JackPorts* deck_b = jack_client->createOutputPorts("deck_b");
    JackModule* decks_fader = new JackFaderModule("decks_fader", jack_client);
    deck_a->connectTo(decks_fader);
    deck_b->connectTo(decks_fader);
    decks_fader->connectTo(master_fader);
    decks_fader->activate();

    JackPorts* fx = jack_client->createOutputPorts("fx");
    JackModule* fx_fader = new JackFaderModule("fx_fader", jack_client);
    fx->connectTo(fx_fader);
    fx_fader->connectTo(master_fader);
    fx_fader->activate();

    recorder->startRecording("/tmp/picaster.flac");
    int bloc = 500;
    while (--bloc > 0)
    {
        usleep(50000);
    }

    recorder->stopRecording();
    
    jack_client->close();

    return 0;
}