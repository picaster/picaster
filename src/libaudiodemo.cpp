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

#include "JackClient.h"
#include "JackRecorderModule.h"
#include "JackFaderModule.h"
#include "JackFilePlayerModule.h"
#include "ShoutcastStreamerModule.h"

bool finished = false;

static void
playerCallback(int64_t position, void* user_data)
{
    JackFilePlayerModule* deck = (JackFilePlayerModule*)user_data;
    char* formated_position = deck->formatTime(position);
    std::cerr << "Tick : " << position << " (" << formated_position << ")" << std::endl;
    delete formated_position;
}

int
main(int argc, char** argv)
{
    JackClient* jack_client = JackClient::getInstance("PiCaster");

    jack_client->setJackdPath("/usr/bin/jackd");
    jack_client->setDriver("alsa");
    jack_client->setSampleRate("48000");
    jack_client->setFramesPerPeriod("512");
    jack_client->setPeriodsPerBuffer("3");
    jack_client->setInputDevice("CODEC");
    jack_client->setOutputDevice("CODEC");
    jack_client->startJack();

    JackRecorderModule* recorder = new JackRecorderModule("recorder", jack_client);
    ShoutcastStreamerModule* streamer = new ShoutcastStreamerModule("streamer", jack_client);
    JackFaderModule* master_fader = new JackFaderModule("master_fader", jack_client);
    JackModule* dj_fader = new JackFaderModule("dj_fader", jack_client);
    JackFilePlayerModule* deck_a = new JackFilePlayerModule("deck_a", jack_client);
    JackFilePlayerModule* deck_b = new JackFilePlayerModule("deck_b", jack_client);
    JackModule* decks_fader = new JackFaderModule("decks_fader", jack_client);
    JackPorts* fx = jack_client->createOutputPorts("fx");
    JackModule* fx_fader = new JackFaderModule("fx_fader", jack_client);

    JackPorts* playback_ports = jack_client->getPlaybackPorts();
    JackPorts* capture_ports = jack_client->getCapturePorts();

    master_fader->setSliderValue(1.0); /* 0 dBFS */
    master_fader->connectTo(playback_ports);
    master_fader->connectTo(recorder);
    master_fader->connectTo(streamer);

    capture_ports->connectTo(dj_fader);
    dj_fader->connectTo(master_fader);

    deck_a->connectTo(decks_fader);
    deck_b->connectTo(decks_fader);
    decks_fader->connectTo(master_fader);

    fx->connectTo(fx_fader);
    fx_fader->connectTo(master_fader);

    recorder->startRecording("/tmp/picaster.flac");
    //deck_a->load("/home/yannick/Téléchargements/Kwizat_Haterach_-_Le_bon_moment.flac");
    //deck_a->playFile("/data/Musique/picaster/stereo_square_4hz_0.8.flac");
    //deck_a->playFile("/home/yannick/Musique/demo.flac");
    //deck_a->load("/data/Musique/349863__jtnewlin13__leather-jacket-wooshes.wav");
    deck_a->load("/home/yannick/Musique/JekK_-_Strong.mp3");

    int64_t duration = deck_a->getDuration();
    char* formated_duration = deck_a->formatTime(duration);
    std::cerr << "Duration : " << duration << " (" << formated_duration << ")" << std::endl;
    delete formated_duration;

    streamer->connect("stream.euterpia-radio.fr", 8902, "GslE7x2k");

    deck_a->play(playerCallback, deck_a);

    while (deck_a->isPlaying())
    {
        usleep(500000);
        int64_t position = deck_a->getPosition();
        char* formated_position = deck_a->formatTime(position);
        std::cerr << "Tick : " << position << " (" << formated_position << ")" << std::endl;
        delete formated_position;
   }

    recorder->stopRecording();
    
    jack_client->stopJack();

    return 0;
}