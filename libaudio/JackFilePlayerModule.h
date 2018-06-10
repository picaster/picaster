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

#ifndef __JACK_FILE_PLAYER_MODULE_H_INCLUDED
#define __JACK_FILE_PLAYER_MODULE_H_INCLUDED

#include <pthread.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

#include "JackModule.h"

#include "Float32RingBuffer.h"

class JackFilePlayerModule : public JackModule
{
    private:
        int out_channels;
        int out_samples;
        int sample_rate;
        int max_buffer_size;
        AVFormatContext* fmt_ctx;
        bool playing;
        pthread_t thread_id;
        pthread_mutex_t disk_thread_lock = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t data_ready = PTHREAD_COND_INITIALIZER;
        Float32RingBuffer* ringbuf;
        bool loaded;
        int64_t duration;
        int64_t position;
        bool stop_requested;

    private:
        static void* playerThreadCallback(void* arg);

    public:
        JackFilePlayerModule(char const* name, JackClient* client);
        void process(jack_nframes_t nframes);
        void load(char const* file_path);
        void play();
        void stop();
        void* playerThread();
        int64_t getDuration();
        int64_t getPosition();
        char* formatTime(int64_t position);
        bool isPlaying();
};

#endif
