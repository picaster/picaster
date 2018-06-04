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
    max_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_samples, AV_SAMPLE_FMT_FLT, 1);
    playing = false;
    ringbuf = jack_ringbuffer_create(sizeof(jack_default_audio_sample_t) * RB_SIZE);
    can_process = false;
    memset (ringbuf->buf, 0, ringbuf->size) ;

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
        return;

    jack_default_audio_sample_t buf[2];
    
    jack_default_audio_sample_t** output_buffers = getOutputPortsBuffers(nframes);

    int total_read_count = 0;
    for (int i = 0; i < (int)nframes; i++)
    {
        size_t read_count = jack_ringbuffer_read(ringbuf, (char*)buf, 2 * sizeof(jack_default_audio_sample_t));
        if (read_count == 0)
        {
            //std::cerr << "PANIC !!!!! read_count == 0" << std::endl;
        }
        else
        {
            //std::cerr << "read_count: " << read_count << std::endl;
        }
        total_read_count += read_count;
        output_buffers[0][i] = buf[0];
        output_buffers[1][i] = buf[1];
    }

    //std::cerr << "total_read_count: " << total_read_count << std::endl;

    /* Wake up the disk thread to read more data. */
    if (pthread_mutex_trylock(&disk_thread_lock) == 0) {
        pthread_cond_signal(&data_ready);
        pthread_mutex_unlock(&disk_thread_lock);
    }
}

void
JackFilePlayerModule::playFile(char const* file_path)
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

    av_dump_format(fmt_ctx, 0, file_path, false);

    pthread_create(&thread_id, NULL, JackFilePlayerModule::playerThreadCallback, this);
}

void*
JackFilePlayerModule::playerThread()
{
    /*
    int out_channels;
    int out_samples;
    int sample_rate;
    int max_buffer_size;
    AVFormatContext* fmt_ctx;
    */

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
    
    uint8_t* buffer = (uint8_t*)av_malloc(max_buffer_size);

    pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL) ;
    pthread_mutex_lock(&disk_thread_lock);

    // read packet from input audio file
    while (av_read_frame(fmt_ctx, &packet) >= 0) {
        std::cerr << "++++++ begin frame " << std::endl;
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
        int got_samples = swr_convert(swr_ctx, &buffer, out_samples, (const uint8_t **)frame->data, frame->nb_samples);
        if (got_samples < 0) {
            std::cerr << "Error: swr_convert()" << std::endl;
            exit(1);
        }

        jack_ringbuffer_data_t vec[2];
        jack_ringbuffer_get_write_vector(ringbuf, vec);

        while (got_samples > 0) {
            int buffer_size = av_samples_get_buffer_size(NULL, out_channels, got_samples, AV_SAMPLE_FMT_FLT, 1);
            size_t buf_avail0 = vec[0].len;
            size_t buf_avail1 = vec[1].len;
            int buf_avail = buf_avail0 + buf_avail1;
            std::cerr << "got_samples: " << got_samples << std::endl;
            std::cerr << "buffer_size: " << buffer_size << std::endl;
            std::cerr << "buf_avail  : " << buf_avail << std::endl;
            std::cerr << "buf_avail0 : " << buf_avail0 << std::endl;
            std::cerr << "buf_avail1 : " << buf_avail1 << std::endl;

            if (buf_avail0 < (size_t) buffer_size)
            {
                fprintf(stderr, "------------------------------\n");
                memcpy((float*)vec[0].buf, buffer, buf_avail);
                float* a = (float*)buffer;
                float* b = (float*)vec[0].buf;
                for (int i = 0; i < (int)buf_avail; i+=8)
                {
                    fprintf(stderr, "%06d : %5.2f (%5.2f) - %5.2f (%5.2f)\n", offset, *b, *a, *(b + 1), *(a + 1));
                    offset += 1;
                    a += 2;
                    b += 2;
                }
                fprintf(stderr, "- - - - - - - - - - - - - - - \n");
                memcpy((float*)vec[1].buf, buffer + buf_avail, buffer_size - buf_avail);
                b = (float*)vec[1].buf;
                for (int j = 0; j < (int)(buffer_size - buf_avail); j++)
                {
                    fprintf(stderr, "%06d : %5.2f (%5.2f) - %5.2f (%5.2f)\n", offset, *b, *a, *(b + 1), *(a + 1));
                    offset += 1;
                    a += 2;
                    b += 2;
                }
                fprintf(stderr, "------------------------------\n");
            }
            else
            {
                fprintf(stderr, "==============================\n");
                memcpy((float*)vec[0].buf, buffer, buffer_size);
                float* a = (float*)buffer;
                float* b = (float*)vec[0].buf;
                for (int i = 0; i < (int)buffer_size; i+=8)
                {
                    fprintf(stderr, "%06d : %5.2f (%5.2f) - %5.2f (%5.2f)\n", offset, *b, *a, *(b + 1), *(a + 1));
                    offset += 1;
                    a += 2;
                    b += 2;
                }
                fprintf(stderr, "==============================\n");
            }
            jack_ringbuffer_write_advance(ringbuf, buffer_size);

            /*
            float* b = (float*)buffer;
            for (int i = 0; i < got_samples; i += 1)
            {
                fprintf(stderr, "%06d : %5.2f - %5.2f\n", offset, *(b + 2 * i), *(b + 2 * i + 1));
                offset += 1;
            }
            */
            // process samples buffered inside swr context
            got_samples = swr_convert(swr_ctx, &buffer, out_samples, NULL, 0);
            if (got_samples < 0) {
                std::cerr << "Error: swr_convert()" << std::endl;
                exit(1);
            }
        }

        // free packet created by decoder
        av_free_packet(&packet);

        std::cerr << "++++++ end frame " << std::endl;
        can_process = true;
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