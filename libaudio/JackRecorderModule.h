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

#ifndef __JACK_RECORDER_MODULE_H_INCLUDED
#define __JACK_RECORDER_MODULE_H_INCLUDED

#include <sndfile.h>
#include <jack/jack.h>
#include <jack/ringbuffer.h>

#include "JackModule.h"
#include "JackClient.h"

class JackRecorderModule : public JackModule
{
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
        static void* captureThreadCallback(void* arg);

    public:
        JackRecorderModule(const char* name, JackClient* client);
        void process(jack_nframes_t nframes);
        bool startRecording(const char* filepath);
        void* captureThread();
        bool stopRecording();
};

#endif