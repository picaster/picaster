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

#include <stdlib.h>

#include "JackPorts.h"
#include "JackModule.h"

JackPorts::JackPorts(jack_client_t* client, jack_port_t* port1, jack_port_t* port2, JackPortType type)
{
    this->client = client;
    this->port1 = port1;
    this->port2 = port2;
    this->type = type;
    buffers = (jack_default_audio_sample_t**)malloc(2 * sizeof(jack_default_audio_sample_t*));
}

void
JackPorts::connectTo(JackPorts* dest)
{
    if (type == dest->type)
    {
        return;
    }

    jack_connect(client, jack_port_name(port1), jack_port_name(dest->port1));
    jack_connect(client, jack_port_name(port2), jack_port_name(dest->port2));
}

void
JackPorts::connectTo(JackModule* module)
{
    connectTo(module->getInputPorts());
}

jack_default_audio_sample_t**
JackPorts::getBuffers(jack_nframes_t nframes)
{
    buffers[0] = (jack_default_audio_sample_t*)jack_port_get_buffer(port1, nframes);
    buffers[1] = (jack_default_audio_sample_t*)jack_port_get_buffer(port2, nframes);
    return buffers;
}
