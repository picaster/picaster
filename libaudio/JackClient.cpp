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

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "JackClient.h"
#include "JackModule.h"
#include "JackRecorderModule.h"

#include <jack/jack.h>

// -- Static callbacks

int
JackClient::process_callback(jack_nframes_t nframes, void* arg)
{
    JackClient* jackClient = (JackClient*)arg;
    return jackClient->process(nframes);
}

void
JackClient::shutdown_callback(void* arg)
{
    std::cout << "JACK shutdown, exiting." << std::endl;
    exit(1);
}

void
JackClient::signal_handler(int sig)
{
    //jack_client_close(client);
    std::cerr << "Signal received, exiting." << std::endl;
    exit(0);
}

// -- Methods

JackClient::JackClient()
{
    jack_options_t options = JackNoStartServer;
    jack_status_t status;
    client = jack_client_open("PiCaster", options, &status);
    len_modules = 10;
    nb_modules = 0;
    modules = (JackModule**)malloc(len_modules * sizeof(JackModule*));
    can_capture = false;
}

JackPorts*
JackClient::createOutputPorts(const char* name)
{
    return createPorts(name, OUTPUT_PORT);
}

JackPorts*
JackClient::createInputPorts(const char* name)
{
    return createPorts(name, INPUT_PORT);
}

JackPorts*
JackClient::getPlaybackPorts()
{
    jack_port_t* port1 = jack_port_by_name(client, "system:playback_1");
    jack_port_t* port2 = jack_port_by_name(client, "system:playback_2");

    return new JackPorts(this->client, port1, port2, INPUT_PORT);
}

JackPorts*
JackClient::getCapturePorts()
{
    jack_port_t* port1 = jack_port_by_name(client, "system:capture_1");
    jack_port_t* port2 = jack_port_by_name(client, "system:capture_2");

    return new JackPorts(this->client, port1, port2, OUTPUT_PORT);
}

void
JackClient::close()
{
    jack_client_close(client);
}

jack_client_t*
JackClient::getClient()
{
    return this->client;
}

void
JackClient::registerModule(JackModule* module)
{
    modules[nb_modules++] = module;
    if (nb_modules >= len_modules) {
        len_modules *= 2;
        modules = (JackModule**)realloc(modules, len_modules * sizeof(JackModule*));
    }    
}

// -- Private

JackPorts*
JackClient::createPorts(const char* name, JackPortType type)
{
    char* port_name = (char*)calloc(strlen(name) + 7, sizeof(char));

    sprintf(port_name, type == OUTPUT_PORT ? "%s_out_1" : "%s_in_1", name);
    jack_port_t* port1 = jack_port_register(client, port_name, JACK_DEFAULT_AUDIO_TYPE, type == OUTPUT_PORT ? JackPortIsOutput : JackPortIsInput, 0);

    sprintf(port_name, type == OUTPUT_PORT ? "%s_out_2" : "%s_in_2", name);
    jack_port_t* port2 = jack_port_register(client, port_name, JACK_DEFAULT_AUDIO_TYPE, type == OUTPUT_PORT ? JackPortIsOutput : JackPortIsInput, 0);

    free(port_name);

    return new JackPorts(this->client, port1, port2, type);
}

int
JackClient::process(jack_nframes_t nframes)
{
    for (int i = 0; i < nb_modules; i++)
    {
        modules[i]->process(nframes);
    }
    return 0;
}

void
JackClient::activate()
{
    if (this->client != NULL)
    {
        jack_set_process_callback(client, JackClient::process_callback, this);
        jack_on_shutdown(client, JackClient::shutdown_callback, NULL);
        signal(SIGQUIT, JackClient::signal_handler);
        signal(SIGHUP, JackClient::signal_handler);
        signal(SIGTERM, JackClient::signal_handler);
        signal(SIGINT, JackClient::signal_handler);

        jack_activate(this->client);
    }
}
