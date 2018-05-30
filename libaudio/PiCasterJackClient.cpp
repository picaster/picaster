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
        const size_t sample_size = sizeof(jack_default_audio_sample_t);

    private:
        bool               recording;
        pthread_t          capture_thread_id;
        SNDFILE*           capture_sf;
        pthread_mutex_t    disk_thread_lock = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t     data_ready = PTHREAD_COND_INITIALIZER;
        jack_ringbuffer_t* rb;

    private:
        static void* captureThreadCallback(void* arg)
        {
            PiCasterDiskRecorderModule* disk_recorder_module = (PiCasterDiskRecorderModule*)arg;
            return disk_recorder_module->captureThread();
        }

    public:
        PiCasterDiskRecorderModule(char* name, JackPorts* input_ports, JackPorts* output_ports) : JackModule(name, input_ports, output_ports) 
        {
            recording = false;
            rb = jack_ringbuffer_create(2 * sample_size * 8192);
        }

        void process(jack_nframes_t nframes)
        {
            if (recording)
            {
                jack_default_audio_sample_t** input_buffers = getInputPortsBuffers(nframes);

        	    /* Sndfile requires interleaved data.  It is simpler here to
            	 * just queue interleaved samples to a single ringbuffer. */
                for (jack_nframes_t frame = 0; frame < nframes; frame++)
                {
                    for (int chn = 0; chn < 2; chn++) {
                        jack_ringbuffer_write(rb, (const char*)(input_buffers[chn] + frame), sample_size);
                    }
                }

                /* Tell the disk thread there is work to do.  If it is already
                * running, the lock will not be available.  We can't wait
                * here in the process() thread, but we don't need to signal
                * in that case, because the disk thread will read all the
                * data queued before waiting again. */
                if (pthread_mutex_trylock(&disk_thread_lock) == 0)
                {
                    pthread_cond_signal(&data_ready);
                    pthread_mutex_unlock(&disk_thread_lock);
                }
            }
        }

        bool startRecording(jack_client_t* client, const char* filepath)
        {
            SF_INFO sf_info;
            sf_info.samplerate = jack_get_sample_rate(client);
            sf_info.format = SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
            sf_info.channels = 2;
            if ((capture_sf = sf_open(filepath, SFM_WRITE, &sf_info)) == NULL) {
                char errstr[256];
                sf_error_str(0, errstr, sizeof (errstr) - 1);
                std::cerr << "Cannot open sndfile " << filepath << " for output : " << errstr << std::endl;
                jack_client_close(client);
                exit(1);
            }
            
            recording = true;
            pthread_create(&capture_thread_id, NULL, PiCasterDiskRecorderModule::captureThreadCallback, this);

            return true;
        }

        void* captureThread()
        {
            size_t bytes_per_frame = 2 * sample_size;
            void *framebuf = malloc(bytes_per_frame);

            while (recording)
            {
                while (jack_ringbuffer_read_space(rb) >= bytes_per_frame)
                {
                    jack_ringbuffer_read(rb, (char*)framebuf, bytes_per_frame);
                    if (sf_writef_float(capture_sf, (float*)framebuf, 1) != 1) {
                        char errstr[256];
                        sf_error_str (0, errstr, sizeof (errstr) - 1);
                        cerr << "Cannot write sndfile : " << errstr << endl;
                        exit(1);
                    }
                }
        		/* wait until process() signals more data */
		        pthread_cond_wait(&data_ready, &disk_thread_lock);
            }
            cerr << "captureThread: done." << endl;
            pthread_mutex_unlock(&disk_thread_lock);
            free(framebuf);
            return NULL;
        }

        bool stopRecording(jack_client_t* client)
        {
            recording = false;
            sf_close(capture_sf);
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
