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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> //defines IPPROTO_TCP on BSD
#include <netdb.h>
#include <sys/select.h>
#include <errno.h>
#include <iostream>

#include "ShoutcastStreamerModule.h"

void*
ShoutcastStreamerModule::streamThreadCallback(void* arg)
{
    ShoutcastStreamerModule* module = (ShoutcastStreamerModule*)arg;
    return module->streamThread();
}

ShoutcastStreamerModule::ShoutcastStreamerModule(const char* name, JackClient* client) : JackModule(name, client)
{
    input_ports = client->createInputPorts(name);
    connected = false;
    ringbuf = jack_ringbuffer_create(RB_SIZE);
    client->registerModule(this);
}

void
ShoutcastStreamerModule::process(jack_nframes_t nframes)
{/*
    if (connected)
    {
        jack_default_audio_sample_t** input_ports = getInputPortsBuffers(nframes);
        for (jack_nframes_t frame = 0; frame < nframes; frame++)
        {
            for (int chn = 0; chn < 2; chn++)
            {
                jack_ringbuffer_write(ringbuf, (const char*)(input_buffers[chn] + frame), sample_size);
            }
        }

        if (pthread_mutex_trylock(&mutex) == 0)
        {
            pthread_cond_signal(&data_ready);
            pthread_mutex_unlock(&mutex);
        }
    }*/
}

int
ShoutcastStreamerModule::connect(char const* server_name, short port, char const* password)
{
    static bool error_printed = 0;
    int ret;
    char recv_buf[100];
    char send_buf[100];

    stream_socket = sock_connect(server_name, port + 1, CONN_TIMEOUT);
    if (stream_socket < 0)
    {
        switch (stream_socket)
        {
            case SOCK_ERR_CREATE:
                if (!error_printed)
                {
                    std::cerr << std::endl << "Connect: Could not create network socket" << std::endl;
                    error_printed = 1;
                }
                ret = 2;
                break;
            case SOCK_ERR_RESOLVE:
                if(!error_printed)
                {
                    std::cerr << std::endl << "Connect: Error resolving server address" << std::endl;
                    error_printed = 1;
                }
                ret = 1;
                break;
            case SOCK_TIMEOUT:
            case SOCK_INVALID:
                ret = 1;
                break;
            default:
                ret = 2;
        }

        this->disconnect();
        return ret;
    }

    error_printed = 0;
    
    snprintf(send_buf, sizeof(send_buf), "%s%s", password, "\r\n");
    sock_send(&stream_socket, send_buf, strlen(send_buf), SEND_TIMEOUT);
    
    // Make picaster compatible to proxies/load balancers. Thanks to boyska
    if (port == 80)
        snprintf(send_buf, sizeof(send_buf), "Host: %s\r\n", server_name);
    else
        snprintf(send_buf, sizeof(send_buf), "Host: %s:%d\r\n", server_name, port);
    sock_send(&stream_socket, send_buf, strlen(send_buf), SEND_TIMEOUT);

#if 0
   if(cfg.main.num_of_icy > 0)
    {
        send_icy_header((char*)"icy-name", cfg.icy[cfg.selected_icy]->name);
        send_icy_header((char*)"icy-genre", cfg.icy[cfg.selected_icy]->genre);
        send_icy_header((char*)"icy-url", cfg.icy[cfg.selected_icy]->url);
        send_icy_header((char*)"icy-irc", cfg.icy[cfg.selected_icy]->irc);
        send_icy_header((char*)"icy-icq", cfg.icy[cfg.selected_icy]->icq);
        send_icy_header((char*)"icy-aim", cfg.icy[cfg.selected_icy]->aim);
        send_icy_header((char*)"icy-pub", cfg.icy[cfg.selected_icy]->pub);
    }
    else
    {
#endif
        send_icy_header((char*)"icy-name", (char*)"PiCaster");
        send_icy_header((char*)"icy-pub", (char*)"0");
#if 0
    }
#endif

    snprintf(send_buf, sizeof(send_buf), "%u", /*cfg.audio.bitrate*/128);
    send_icy_header((char*)"icy-br", send_buf);
    
    sock_send(&stream_socket, "content-type:", 13, SEND_TIMEOUT);

    strcpy(send_buf, "audio/mpeg");
    //strcpy(send_buf,  "audio/aac");
    //strcpy(send_buf,  "audio/ogg");

    sock_send(&stream_socket, send_buf, strlen(send_buf), SEND_TIMEOUT);
   
    sock_send(&stream_socket, "\r\n\r\n", 4, SEND_TIMEOUT);

    if((ret = sock_recv(&stream_socket, recv_buf, sizeof(recv_buf)-1, RECV_TIMEOUT)) == 0)
    {
        usleep(100*1000);
        this->disconnect();
        return 1;
    }

    recv_buf[ret] = '\0';
    
    if( (recv_buf[0] != 'O') || (recv_buf[1] != 'K') || (ret <= 2) )
    {
        if(strstr(recv_buf, "invalid password") != NULL || (strstr(recv_buf, "Invalid Password") != NULL))
        {
            std::cerr << std::endl << "Connect: Invalid password!" << std::endl;
            this->disconnect();
            return 2;
        }
        return 1;
    }

    connected = 1;
    pthread_create(&thread_id, NULL, ShoutcastStreamerModule::streamThreadCallback, this);

    //timer_init(&stream_timer, 1);       //starts the "online" timer

    return 0;
}

void 
ShoutcastStreamerModule::disconnect()
{
    connected = 0;
    sock_close(&stream_socket);
}

void*
ShoutcastStreamerModule::streamThread()
{/*
    size_t bytes_per_frame = 2 * sample_size;
    void *framebuf = malloc(bytes_per_frame);

    // find the MP2 encoder
    AVCodec* codec = avcodec_find_encoder(CODEC_ID_MP3);
    if (!codec) {
        std::cerr << "codec not found" << std:endl;
        exit(1);
    }

    AVCodecContext* c = avcodec_alloc_context();
    // put sample parameters
    c->bit_rate = 96
    c->sample_rate = 44100;
    c->channels = 2;

    // open it
    if (avcodec_open(c, codec) < 0) {
        std::cerr << "could not open codec" << std:endl;
        exit(1);
    }

    // the codec gives us the frame size, in samples
    int frame_size = c->frame_size;
    short* samples = malloc(frame_size * 2 * c->channels);


    while (connected)
    {
        while (jack_ringbuffer_read_space(rb) >= bytes_per_frame)
        {

        }
    }
*/
    return NULL;
}

void
ShoutcastStreamerModule::send_icy_header(char* key, char* val)
{
    char *icy_line;
    int len;

    if ((val == NULL) || (strlen(val) == 0))
        return;

    len = strlen(key) + strlen(val) + strlen(":\r\n") + 1;
    icy_line = (char*)malloc(len * sizeof(char) + 1);
    snprintf(icy_line, len, "%s:%s\r\n", key, val);
    
    sock_send(&stream_socket, icy_line, strlen(icy_line), SEND_TIMEOUT);

    free(icy_line);
}

int
ShoutcastStreamerModule::sock_connect(char const* addr, short port, int timeout_ms)
{
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(sock == -1)
        return SOCK_ERR_CREATE;

    struct hostent* host_ptr = gethostbyname(addr);
    if(host_ptr == NULL)
    {
        host_ptr = gethostbyaddr(addr, strlen(addr), AF_INET);
        if(host_ptr == NULL)
        {
            sock_close(&sock);
            return SOCK_ERR_RESOLVE;
        }
    }

    struct sockaddr_in hostname;
    hostname.sin_family = AF_INET;
    hostname.sin_port = htons(port);

    memcpy(&hostname.sin_addr, host_ptr->h_addr, host_ptr->h_length);

    sock_nonblock(&sock);

    ::connect(sock, (struct sockaddr*)&hostname, sizeof(hostname));

    if (sock_select(&sock, timeout_ms, WRITE) <= 0)
    {
        sock_close(&sock);
        return SOCK_TIMEOUT;
    }

    if (!sock_isvalid(&sock))
    {
        sock_close(&sock);
        return SOCK_INVALID;
    }

    return sock;    
}

void
ShoutcastStreamerModule::sock_close(int* s)
{
    close(*s);
}

int
ShoutcastStreamerModule::sock_nonblock(int* s)
{
    long arg;

    arg = fcntl(*s, F_GETFL);
    arg |= O_NONBLOCK;

    return fcntl(*s, F_SETFL, arg);
}

int
ShoutcastStreamerModule::sock_select(int* s, int timout_ms, int mode)
{
   struct timeval tv;
   fd_set fd_wr, fd_rd;

   FD_ZERO(&fd_wr);
   FD_ZERO(&fd_rd);
   FD_SET(*s, &fd_wr);
   FD_SET(*s, &fd_rd);

   tv.tv_sec = timout_ms / 1000;
   tv.tv_usec = (timout_ms % 1000) * 1000;

   switch (mode)
   {
       case READ:
           return select(*s + 1, &fd_rd, NULL, NULL, &tv);
           break;
       case WRITE:
           return select(*s + 1, NULL, &fd_wr, NULL, &tv);
           break;
       case RW:
           return select(*s + 1, &fd_rd, &fd_wr, NULL, &tv);
           break;
       default:
           return SOCK_NO_MODE;
   }
}

int
ShoutcastStreamerModule::sock_isvalid(int* s)
{
    int optval;
    socklen_t len = sizeof(optval);

    getsockopt(*s, SOL_SOCKET, SO_ERROR, (char*)(&optval), &len);

    if (optval)
        return 0;

    return 1;
}

int
ShoutcastStreamerModule::sock_send(int* s, const char* buf, int len, int timout_ms)
{
    int rc;
    int sent = 0;
    int error;

    while (sent < len)
    {
        rc = sock_select(s, 10000, WRITE); //check if socket is connected
        
        if (rc == 0)
        {
            fflush(stdout);
            return SOCK_TIMEOUT;
        }

        if (rc == -1)
        {
            fflush(stdout);
            return SOCK_TIMEOUT;
        }

        if ((rc = send(*s, buf + sent, len - sent, 0)) < 0)
        {
            
            error = errno;
            if (error != EWOULDBLOCK)
            {
                fflush(stdout);
                return SOCK_TIMEOUT;
            }
            rc = 0;
        }

        sent += rc;
    }
    return sent;
}

int
ShoutcastStreamerModule::sock_recv(int *s, char *buf, int len, int timout_ms)
{
    int rc;

    if(sock_select(s, timout_ms, READ) <= 0)
        return SOCK_TIMEOUT;

    rc = recv(*s, buf, len, 0);

    return rc;
}

