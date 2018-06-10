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
#include <math.h>
#include <string.h>

#include "JackFaderModule.h"

JackFaderModule::JackFaderModule(const char* name, JackClient* client) : JackModule(name, client)
{
    input_ports = client->createInputPorts(name);
    output_ports = client->createOutputPorts(name);
    setSliderValue(0.89917899646); /* -3 dBFS by default */
    enabled = true;
    client->registerModule(this);
}

void
JackFaderModule::process(jack_nframes_t nframes)
{
    jack_default_audio_sample_t** input_buffers = getInputPortsBuffers(nframes);
    jack_default_audio_sample_t** output_buffers = getOutputPortsBuffers(nframes);

    for (jack_nframes_t frame = 0; frame < nframes; frame++)
    {
        float a = enabled ? amplification : 0;
        if (this->enabled)
        {
            output_buffers[0][frame] = a * input_buffers[0][frame];
            output_buffers[1][frame] = a * input_buffers[1][frame];
        }
        else
        {
            output_buffers[0][frame] = 0;
            output_buffers[1][frame] = 0;
        }
    }
}

void
JackFaderModule::setSliderValue(double slider_value)
{
    this->slider_value = slider_value; 
    db_value = 65 * log10(slider_value);
    amplification = pow(10, db_value / 20.0f);
}

void
JackFaderModule::enable(bool enabled)
{
    this->enabled = enabled;
}
