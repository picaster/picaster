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

#include "JackModule.h"
#include "JackPorts.h"

JackModule::JackModule(JackPorts* inputPorts, JackPorts* outputPorts)
{
    this->inputPorts = inputPorts;
    this->outputPorts = outputPorts;
}

void
JackModule::connectTo(JackPorts* inputPorts)
{
    outputPorts->connectTo(inputPorts);
}

void
JackModule::connectTo(JackModule* module)
{
    outputPorts->connectTo(module->getInputPorts());
}

JackPorts*
JackModule::getInputPorts()
{
    return inputPorts;
}

JackPorts*
JackModule::getOutputPorts()
{
    return outputPorts;
}

void
JackModule::process(jack_nframes_t nframes)
{
    jack_default_audio_sample_t** input_buffers = inputPorts->getBuffers(nframes);
    jack_default_audio_sample_t** output_buffers = outputPorts->getBuffers(nframes);

    for (int frame = 0; frame < nframes; frame++)
    {
        output_buffers[0][frame] = input_buffers[0][frame];
        output_buffers[1][frame] = input_buffers[1][frame];
    }

    free(input_buffers);
    free(output_buffers);
}