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

#ifndef __JACK_CLIENT_H_INCLUDED
#define __JACK_CLIENT_H_INCLUDED

#include <functional>
#include <jack/jack.h>
#include <jack/ringbuffer.h>
#include <sndfile.h>

#include "JackPorts.h"

class JackModule;
class JackRecorderModule;
class JackModuleFactory;

class JackClient {
    private:
        jack_client_t*     client;
        JackModule**       modules;
        int                nb_modules;
        int                len_modules;
        bool               can_capture;
        pthread_t          capture_thread_id;
        jack_ringbuffer_t* capture_rb;
        SNDFILE*           capture_sf;

    private:
        static int process_callback(jack_nframes_t nframes, void* arg);
        static void shutdown_callback(void* arg);
        static void signal_handler(int sig);
        static void* capture_thread_callback(void* arg);

    private:
        JackPorts* createPorts(const char* name, JackPortType type);
        int process(jack_nframes_t nframes);
        void setupDiskThread();
        void* capture_thread();
    
    public:
        JackClient();
        void activate();
        JackPorts* createOutputPorts(const char* name);
        JackPorts* createInputPorts(const char* name);
        JackPorts* getCapturePorts();
        JackPorts* getPlaybackPorts();
        jack_client_t* getClient();
        void close();
    
    public:
        void registerModule(JackModule* module);
};

#endif
