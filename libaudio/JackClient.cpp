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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <sstream>
#include <string>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "JackClient.h"
#include "JackModule.h"
#include "JackRecorderModule.h"

#include <jack/jack.h>

// -- Static members

JackClient* JackClient::instance;

// -- Static callbacks

int
JackClient::process_callback(jack_nframes_t nframes, void* arg)
{
    JackClient* jackClient = (JackClient*)arg;
    return jackClient->process(nframes);
}

void
JackClient::shutdown_callback(void* arg)
{
    std::cout << "JACK shutdown, exiting." << std::endl;
    exit(1);
}

void
JackClient::signal_handler(int sig)
{
    JackClient* client = JackClient::getInstance(NULL);
    if (client->server_pid > 0)
    {
        kill(client->server_pid, SIGKILL);
    }
    exit(0);
}

int
JackClient::xrun_callback(void* arg)
{
    std::cout << "JACK xrun..." << std::endl;
    //JackClient* client = (JackClient*)arg;
    return 0;
}

// -- Static methods

JackClient*
JackClient::getInstance(const char* client_name)
{
    if (JackClient::instance == NULL)
    {
        JackClient::instance = new JackClient(client_name);
    }
    return JackClient::instance;
}

// -- Methods

JackPorts*
JackClient::createOutputPorts(const char* name)
{
    return createPorts(name, OUTPUT_PORT);
}

JackPorts*
JackClient::createInputPorts(const char* name)
{
    return createPorts(name, INPUT_PORT);
}

JackPorts*
JackClient::getPlaybackPorts()
{
    jack_port_t* port1 = jack_port_by_name(client, "system:playback_1");
    jack_port_t* port2 = jack_port_by_name(client, "system:playback_2");

    return new JackPorts(this->client, port1, port2, INPUT_PORT);
}

JackPorts*
JackClient::getCapturePorts()
{
    jack_port_t* port1 = jack_port_by_name(client, "system:capture_1");
    jack_port_t* port2 = jack_port_by_name(client, "system:capture_2");

    return new JackPorts(this->client, port1, port2, OUTPUT_PORT);
}

void
JackClient::stopJack()
{
    if (client == NULL) return;
    if (!detached)
    {
        kill(server_pid, SIGKILL);
    }
    jack_client_close(client);
    client = NULL;
}

jack_client_t*
JackClient::getClient()
{
    return this->client;
}

void
JackClient::registerModule(JackModule* module)
{
    modules[nb_modules++] = module;
    if (nb_modules >= len_modules) {
        len_modules *= 2;
        modules = (JackModule**)realloc(modules, len_modules * sizeof(JackModule*));
    }    
}

void
JackClient::startJack()
{
    if (client != NULL) return;
    if (!startJackClient(client_name, true))
    {
        char const* params[20];
        std::stringstream cmdline;
#ifdef USE_DUMMY_DRIVER
        cmdline << jackd_path << " -ddummy" << " -p" << frames_per_period << " -r" << sample_rate;
#else
        cmdline << jackd_path << " -d" << driver_name << " -n" << periods_per_buffer << " -Chw:" << input_device << " -Phw:" << output_device << " -p" << frames_per_period << " -r" << sample_rate;
#endif
        char* c = strdup(cmdline.str().c_str());
        char* b = c;
        int p = 0;
        params[p++] = b;
        while (*b != 0)
        {
            if (*b == ' ')
            {
                *b = 0;
                params[p++] = b + 1;
            }
            b += 1;
        }
        params[p++] = 0;
        for (int i = 0; i < p; i++)
        {
            std::cout << params[i] << " ";
        }
        std::cout << std::endl;

        // Fork jackd
        server_pid = fork();
        if (server_pid == 0)
        {
            // child
            execv(params[0], (char* const*)params);
            delete(c);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            startJackClient(client_name, false);
        }
    }
}

void
JackClient::setJackdPath(char const* jackd_path)
{
    this->jackd_path = jackd_path;
}

void
JackClient::setDriver(char const* driver_name)
{
    this->driver_name = driver_name;
}

void
JackClient::setSampleRate(char const* sample_rate)
{
    this->sample_rate = sample_rate;
}

void
JackClient::setFramesPerPeriod(char const* frames_per_period)
{
    this->frames_per_period = frames_per_period;
}

void
JackClient::setPeriodsPerBuffer(char const* periods_per_buffer)
{
    this->periods_per_buffer = periods_per_buffer;
}

void
JackClient::setInputDevice(char const* input_device)
{
    this->input_device = input_device;
}

void
JackClient::setOutputDevice(char const* output_device)
{
    this->output_device = output_device;
}

// -- Private

JackClient::JackClient(const char* client_name)
{
    this->client_name = client_name;
    len_modules = 10;
    nb_modules = 0;
    modules = (JackModule**)malloc(len_modules * sizeof(JackModule*));
    can_capture = false;
    client = NULL;
    startJackClient(client_name, true);
}


bool
JackClient::startJackClient(const char* client_name, bool detached)
{
    jack_options_t options = JackNoStartServer;
    jack_status_t status;
    client = jack_client_open(client_name, options, &status);
    if (client == NULL)
    {
        if (!detached)
        {
            /* TODO: handle error messages here */
        }
        return false;
    }
    jack_set_xrun_callback(client, JackClient::xrun_callback, this);
    jack_on_shutdown(client, JackClient::shutdown_callback, this);

    jack_set_process_callback(client, JackClient::process_callback, this);
    signal(SIGQUIT, JackClient::signal_handler);
    signal(SIGHUP, JackClient::signal_handler);
    signal(SIGTERM, JackClient::signal_handler);
    signal(SIGINT, JackClient::signal_handler);

    jack_activate(client);

    this->detached = detached;

    return true;
}

JackPorts*
JackClient::createPorts(const char* name, JackPortType type)
{
    char* port_name = (char*)calloc(strlen(name) + 7, sizeof(char));

    sprintf(port_name, type == OUTPUT_PORT ? "%s_out_1" : "%s_in_1", name);
    jack_port_t* port1 = jack_port_register(client, port_name, JACK_DEFAULT_AUDIO_TYPE, type == OUTPUT_PORT ? JackPortIsOutput : JackPortIsInput, 0);

    sprintf(port_name, type == OUTPUT_PORT ? "%s_out_2" : "%s_in_2", name);
    jack_port_t* port2 = jack_port_register(client, port_name, JACK_DEFAULT_AUDIO_TYPE, type == OUTPUT_PORT ? JackPortIsOutput : JackPortIsInput, 0);

    free(port_name);

    return new JackPorts(this->client, port1, port2, type);
}

int
JackClient::process(jack_nframes_t nframes)
{
    for (int i = 0; i < nb_modules; i++)
    {
        modules[i]->process(nframes);
    }
    return 0;
}
