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
        static JackClient* instance;

    private:
        jack_client_t*     client;
        JackModule**       modules;
        int                nb_modules;
        int                len_modules;
        bool               can_capture;
        pthread_t          capture_thread_id;
        jack_ringbuffer_t* capture_rb;
        SNDFILE*           capture_sf;
        bool               detached;
        const char*        client_name;
        pid_t              server_pid;
        char const*        jackd_path;
        char const*        driver_name;
        char const*        sample_rate;
        char const*        frames_per_period;
        char const*        periods_per_buffer;
        char const*        input_device;
        char const*        output_device;

    private:
        static int process_callback(jack_nframes_t nframes, void* arg);
        static void shutdown_callback(void* arg);
        static int xrun_callback(void* arg);
        static void signal_handler(int sig);
        static void* capture_thread_callback(void* arg);

    private:
        JackClient(const char* client_name);
        JackPorts* createPorts(const char* name, JackPortType type);
        int process(jack_nframes_t nframes);
        void setupDiskThread();
        void* capture_thread();
        void initCallbacksAndSignals();
    
    public:
        bool startJackClient(const char* client_name, bool detached);
        void setJackdPath(char const* jackd_path);
        void setDriver(char const* driver_name);
        void setSampleRate(char const* sample_rate);
        void setFramesPerPeriod(char const* frames_per_period);
        void setPeriodsPerBuffer(char const* periods_per_buffer);
        void setInputDevice(char const* input_device);
        void setOutputDevice(char const* output_device);
        void startJack(void (*callback)());
        JackPorts* createOutputPorts(const char* name);
        JackPorts* createInputPorts(const char* name);
        JackPorts* getCapturePorts();
        JackPorts* getPlaybackPorts();
        jack_client_t* getClient();
        void stopJack();
        void registerModule(JackModule* module);
        bool isConnected();
        
    public:
        static JackClient* getInstance(const char* client_name);
};

#endif
