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
#include <stdlib.h>

#include "PiCasterJackClient.h"
#include "JackModule.h"

using namespace std;

class PiCasterDiskRecorderModule : public JackModule {
    private:
        bool recording;

    public:
        PiCasterDiskRecorderModule(char* name, JackPorts* input_ports, JackPorts* output_ports) : JackModule(name, input_ports, output_ports) 
        {
            recording = false;
        }

        void process(jack_nframes_t nframes)
        {
            if (recording)
            {
                jack_default_audio_sample_t** input_buffers = getInputPortsBuffers(nframes);

                for (int frame = 0; frame < nframes; frame++)
                {
                }

                free(input_buffers);
            }
        }

        bool startRecording(jack_client_t* client, const char* filepath)
        {
            SF_INFO sf_info;
            sf_info.samplerate = jack_get_sample_rate(client);
            sf_info.format = SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
            sf_info.channels = 2;
            SNDFILE* capture_sf;
            if ((capture_sf = sf_open(filepath, SFM_WRITE, &sf_info)) == NULL) {
                char errstr[256];
                sf_error_str(0, errstr, sizeof (errstr) - 1);
                std::cerr << "Cannot open sndfile " << filepath << " for output : " << errstr << std::endl;
                jack_client_close(client);
                exit(1);
            }
            recording = true;
            //pthread_create(&capture_thread_id, NULL, JackClient::capture_thread_callback, this);

            return true;
        }

        bool stopRecording(jack_client_t* client)
        {
            recording = false;
            return true;
        }
};

class DiskRecorderModuleFactory : public JackModuleFactory {
    public:
        JackModule* newModule(char* name, JackPorts* input_ports, JackPorts* output_ports)
        {
            return new PiCasterDiskRecorderModule(name, input_ports, output_ports);
        }
};

JackModule*
PiCasterJackClient::createDiskRecorderModule()
{
    DiskRecorderModuleFactory* factory = new DiskRecorderModuleFactory();
    disk_recorder_module = (PiCasterDiskRecorderModule*)createModule("disk_recorder", factory, false);
    delete factory;
    return disk_recorder_module;
}
