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

#include "JackClient.h"
#include "JackModule.h"

JackClient::JackClient()
{
    jack_options_t options = JackNoStartServer;
    jack_status_t status;
    this->client = jack_client_open("PiCaster", options, &status);
    if (this->client != NULL)
    {
        std::cout << "Jack client initialized" << std::endl; 
    }
}

void
JackClient::activate()
{
    if (this->client != NULL)
    {
        jack_activate(this->client);
        std::cout << "Jack client activated" << std::endl;
    }
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

JackModule*
JackClient::createModule(const char* name)
{
    JackPorts* portsIn = createInputPorts(name);
    JackPorts* portsOut = createOutputPorts(name);
    return new JackModule(portsIn, portsOut);
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
