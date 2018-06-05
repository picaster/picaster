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

#ifndef __SHOUTCAST_STREAMER_MODULE_H_INCLUDED
#define __SHOUTCAST_STREAMER_MODULE_H_INCLUDED

#include <jack/jack.h>

#include "JackModule.h"

enum {
    READ = 0,
    WRITE = 1,
    RW = 2
};

enum {
    SOCK_ERR_CREATE = -1,
    SOCK_ERR_RESOLVE = -2,
    SOCK_TIMEOUT = -3,
    SOCK_INVALID = -4,
    SOCK_NO_MODE = -5,
    SOCK_ERR_SET_SBUF = -6,
    SOCK_ERR_SET_RBUF = -7
};

enum {
    CONN_TIMEOUT = 1000,
    SEND_TIMEOUT = 3000,
    RECV_TIMEOUT = 1000
};

class ShoutcastStreamerModule : public JackModule
{
    private:
        bool connected;
        bool streaming;
        int stream_socket;

    private:
        int sock_connect(char const* addr, short port, int timeout_ms);
        void sock_close(int* s);
        int sock_nonblock(int* s);
        int sock_select(int* s, int timout_ms, int mode);
        int sock_isvalid(int* s);
        int sock_send(int* s, const char* buf, int len, int timout_ms);
        int sock_recv(int* s, char* buf, int len, int timout_ms);
        void send_icy_header(char* key, char* val);

    public:
        ShoutcastStreamerModule(char const* name, JackClient* client);
        void process(jack_nframes_t nframes);
        int connect(char const* server_name, short port, char const* password);
        void disconnect(void);
};

#endif
