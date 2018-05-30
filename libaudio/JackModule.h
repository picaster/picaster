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

#ifndef __JACK_MODULE_H_INCLUDED
#define __JACK_MODULE_H_INCLUDED

#include <jack/jack.h>

class JackPorts;
class JackModule;

class JackModuleFactory {
    public:
        virtual JackModule* newModule(char* name, JackPorts* input_ports, JackPorts* output_ports);
};


class JackModule {

    private:
        JackPorts* input_ports;
        JackPorts* output_ports;
        char*      name;

    public:
        static JackModuleFactory* DEFAULT_FACTORY;

    public:
        JackModule(char* name, JackPorts* input_ports, JackPorts* output_ports);
        void connectTo(JackPorts* input_ports);
        void connectTo(JackModule* module);
        JackPorts* getInputPorts();
        JackPorts* getOutputPorts();
        jack_default_audio_sample_t** getInputPortsBuffers(jack_nframes_t nframes);
        jack_default_audio_sample_t** getOutputPortsBuffers(jack_nframes_t nframes);

    public:
        virtual void process(jack_nframes_t nframes);
        virtual bool startRecording(jack_client_t* client, const char* filepath);
        virtual bool stopRecording(jack_client_t* client);

};

#endif