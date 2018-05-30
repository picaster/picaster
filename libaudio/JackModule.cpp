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
#include <iostream>

#include "JackModule.h"
#include "JackPorts.h"

JackModuleFactory* JackModule::DEFAULT_FACTORY = new JackModuleFactory();

JackModule*
JackModuleFactory::newModule(char* name, JackPorts* input_ports, JackPorts* output_ports)
{
    return new JackModule(name, input_ports, output_ports);
}

JackModule::JackModule(char* name, JackPorts* input_ports, JackPorts* output_ports)
{
    this->name = name;
    this->input_ports = input_ports;
    this->output_ports = output_ports;
}

void
JackModule::connectTo(JackPorts* input_ports)
{
    output_ports->connectTo(input_ports);
}

void
JackModule::connectTo(JackModule* module)
{
    output_ports->connectTo(module->getInputPorts());
}

JackPorts*
JackModule::getInputPorts()
{
    return input_ports;
}

JackPorts*
JackModule::getOutputPorts()
{
    return output_ports;
}

jack_default_audio_sample_t**
JackModule::getInputPortsBuffers(jack_nframes_t nframes)
{
    return input_ports->getBuffers(nframes);
}

jack_default_audio_sample_t**
JackModule::getOutputPortsBuffers(jack_nframes_t nframes)
{
    return output_ports->getBuffers(nframes);
}

void
JackModule::process(jack_nframes_t nframes)
{
    jack_default_audio_sample_t** input_buffers = getInputPortsBuffers(nframes);
    jack_default_audio_sample_t** output_buffers = getOutputPortsBuffers(nframes);

    for (int frame = 0; frame < nframes; frame++)
    {
        output_buffers[0][frame] = input_buffers[0][frame];
        output_buffers[1][frame] = input_buffers[1][frame];
    }

    free(input_buffers);
    free(output_buffers);
}

bool
JackModule::startRecording(jack_client_t* client, const char* filepath)
{
    return false;
}

bool
JackModule::stopRecording(jack_client_t* client)
{
    return false;
}
