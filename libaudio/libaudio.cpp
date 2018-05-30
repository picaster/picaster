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

#include "libaudio.h"

#include "PiCasterJackClient.h"
#include "JackModule.h"

JackClient*
libaudio_init()
{
    PiCasterJackClient* jack_client = new PiCasterJackClient();
    jack_client->activate();

    JackModule* disk_recorder = jack_client->createDiskRecorderModule();

    JackModule* master_fader = jack_client->createModule("master_fader");
    JackPorts* playback = jack_client->getPlaybackPorts();
    master_fader->connectTo(playback);
    master_fader->connectTo(disk_recorder);

    JackPorts* capture = jack_client->getCapturePorts();
    JackModule* dj_fader = jack_client->createModule("dj_fader");
    capture->connectTo(dj_fader);
    dj_fader->connectTo(master_fader);

    JackPorts* deck_a = jack_client->createOutputPorts("deck_a");
    JackPorts* deck_b = jack_client->createOutputPorts("deck_b");
    JackModule* decks_fader = jack_client->createModule("decks_fader");
    deck_a->connectTo(decks_fader);
    deck_b->connectTo(decks_fader);
    decks_fader->connectTo(master_fader);

    JackPorts* fx = jack_client->createOutputPorts("fx");
    JackModule* fx_fader = jack_client->createModule("fx_fader");
    fx->connectTo(fx_fader);
    fx_fader->connectTo(master_fader);

    return jack_client;
}