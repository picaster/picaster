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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <jack/jack.h>
#include <jack/ringbuffer.h>

#include "JackFilePlayerModule.h"

#define RB_SIZE (1 << 16)

int offset = 0;

// -- Callbacks 

void*
JackFilePlayerModule::playerThreadCallback(void* arg)
{
    JackFilePlayerModule* module = (JackFilePlayerModule*)arg;
    return module->playerThread();
}

JackFilePlayerModule::JackFilePlayerModule(char const* name, JackClient* client) : JackModule(name, client)
{
    output_ports = client->createOutputPorts(name);
    sample_rate = jack_get_sample_rate(client->getClient());
    out_channels = 2;
    out_samples = 4096;
    max_buffer_size = out_channels * out_samples * sizeof(float);
    playing = false;
    can_process = false;
    ringbuf = new Float32RingBuffer(RB_SIZE);
    loaded = false;

    // register supported formats and codecs
    av_register_all();

    // allocate empty format context
    // provides methods for reading input packets
    fmt_ctx = avformat_alloc_context();
    
    client->registerModule(this);
}

void
JackFilePlayerModule::process(jack_nframes_t nframes)
{
    if (!can_process)
    {
        return;
    }

    jack_default_audio_sample_t buf[2];
    
    jack_default_audio_sample_t** output_buffers = getOutputPortsBuffers(nframes);

    int total_read_count = 0;
    for (int i = 0; i < (int)nframes; i++)
    {
        int r_avail = ringbuf->r_buf_avail();
        if (r_avail < 0)
        {
            std::cerr << "[process] PANIC : ring buffer overread" << std::endl;
            exit(1);
        }

        if (r_avail < 2)
        {
            output_buffers[0][i] = 0;
            output_buffers[1][i] = 0;
        }
        else
        {
            int read_count = ringbuf->read(buf, 2);
            total_read_count += read_count;
            output_buffers[0][i] = buf[0];
            output_buffers[1][i] = buf[1];
        }
    }

    /* Wake up the disk thread to read more data. */
    if (pthread_mutex_trylock(&disk_thread_lock) == 0) {
        pthread_cond_signal(&data_ready);
        pthread_mutex_unlock(&disk_thread_lock);
    }
}

void
JackFilePlayerModule::load(char const* file_path)
{
    // determine input file type and initialize format context
    if (avformat_open_input(&fmt_ctx, file_path, NULL, NULL) != 0) {
        std::cerr << "Error: avformat_open_input()" << std::endl;
        exit(1);
    }
    
    // determine supported codecs for input file streams and add
    // them to format context
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        std::cerr << "Error: avformat_find_stream_info()" << std::endl;
        exit(1);
    }

    this->duration = fmt_ctx->duration;

    av_dump_format(fmt_ctx, 0, file_path, false);

    this->loaded = true;
}

int64_t
JackFilePlayerModule::getDuration()
{
    return this->duration;
}

int64_t
JackFilePlayerModule::getPosition()
{
    return this->position;
}

char*
JackFilePlayerModule::formatTime(int64_t position)
{
    int64_t duration = position + (position <= INT64_MAX - 5000 ? 5000 : 0);
    int secs  = duration / AV_TIME_BASE;
    //int us    = duration % AV_TIME_BASE;
    int mins  = secs / 60;
    secs %= 60;
    //int hours = mins / 60;
    //mins %= 60;

    char* formated_duration = new char[9];
    sprintf(formated_duration, "%02d:%02d", mins, secs);

    return formated_duration;
}

void
JackFilePlayerModule::play(JackFilePlayerCallback callback, void* user_data)
{
    if (!this->loaded) return;

    this->callback = callback;
    this->user_data = user_data;
    pthread_create(&thread_id, NULL, JackFilePlayerModule::playerThreadCallback, this);
}

void*
JackFilePlayerModule::playerThread()
{
    // find audio stream in format context
    size_t stream = 0;
    for (; stream < fmt_ctx->nb_streams; stream++) {
        if (fmt_ctx->streams[stream]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            break;
        }
    }
    if (stream == fmt_ctx->nb_streams) {
        std::cerr << "Error: no audio stream found" << std::endl;
        exit(1);
    }

    // get codec context for audio stream
    // provides methods for decoding input packets received from format context
    AVCodecContext* codec_ctx = fmt_ctx->streams[stream]->codec;

    if (codec_ctx->channel_layout == 0) {
        if (codec_ctx->channels == 1)
        {
            codec_ctx->channel_layout = AV_CH_LAYOUT_MONO;
        }
        else
        {
            codec_ctx->channel_layout = AV_CH_LAYOUT_STEREO;
        }
    }

    // find decoder for audio stream
    AVCodec* codec = avcodec_find_decoder(codec_ctx->codec_id);
    if (!codec) {
        std::cerr << "Error: avcodec_find_decoder()" << std::endl;
        exit(1);
    }

    // initialize codec context with decoder we've found
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        std::cerr << "Error: avcodec_open2()" << std::endl;
        exit(1);
    }

    // initialize converter from input audio stream to output stream
    // provides methods for converting decoded packets to output stream
    SwrContext* swr_ctx = swr_alloc_set_opts(
        NULL,
        AV_CH_LAYOUT_STEREO,        // output
        AV_SAMPLE_FMT_FLT,          // output
        sample_rate,                // output
        codec_ctx->channel_layout,  // input
        codec_ctx->sample_fmt,      // input
        codec_ctx->sample_rate,     // input
        0,
        NULL);

    if (!swr_ctx) {
        std::cerr << "Error: swr_alloc_set_opts()" << std::endl;
        exit(1);
    }
    swr_init(swr_ctx);

    // create empty packet for input stream
    AVPacket packet;
    av_init_packet(&packet);
    packet.data = NULL;
    packet.size = 0;    

    // allocate empty frame for decoding
    AVFrame* frame = av_frame_alloc();    

    playing = true;
    
    //uint8_t* buffer = (uint8_t*)av_malloc(max_buffer_size);
    float* buffer = (float*)calloc(max_buffer_size / sizeof(float), sizeof(float));

    pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL) ;
    pthread_mutex_lock(&disk_thread_lock);

    int64_t track_samples = 0;

    // read packet from input audio file
    while (av_read_frame(fmt_ctx, &packet) >= 0) {
        // skip non-audio packets
        if (packet.stream_index != (int)stream) {
            continue;
        }

        // decode packet to frame
        int got_frame = 0;
        if (avcodec_decode_audio4(codec_ctx, frame, &got_frame, &packet) < 0) {
            std::cerr << "Error: avcodec_decode_audio4()" << std::endl;
            exit(1);
        }

        if (!got_frame) {
            continue;
        }

        // convert input frame to output buffer
        int got_samples = swr_convert(swr_ctx, (uint8_t**)&buffer, out_samples, (const uint8_t **)frame->data, frame->nb_samples);
        if (got_samples < 0) {
            std::cerr << "Error: swr_convert()" << std::endl;
            exit(1);
        }

        while (got_samples > 0) {

            track_samples += got_samples;

            int buf_avail = ringbuf->w_buf_avail();

            while (buf_avail < got_samples * 2)
            {
                pthread_cond_wait(&data_ready, &disk_thread_lock);
                buf_avail = ringbuf->w_buf_avail();
            }
            ringbuf->write(buffer, got_samples * 2);

            // process samples buffered inside swr context
            got_samples = swr_convert(swr_ctx, (uint8_t**)&buffer, out_samples, NULL, 0);
            if (got_samples < 0) {
                std::cerr << "Error: swr_convert()" << std::endl;
                exit(1);
            }
        }

        // free packet created by decoder
        av_free_packet(&packet);

        can_process = true;

        position = AV_TIME_BASE * track_samples / this->sample_rate;

        if (this->callback)
        {
            //this->callback(track_position, this->user_data);
        }

        pthread_cond_wait(&data_ready, &disk_thread_lock);
    }

    playing = false;
    pthread_mutex_unlock(&disk_thread_lock);
    
    av_free(buffer);
    av_frame_free(&frame);

    swr_free(&swr_ctx);

    avcodec_close(codec_ctx);
    avformat_close_input(&fmt_ctx);

    return NULL;   
}