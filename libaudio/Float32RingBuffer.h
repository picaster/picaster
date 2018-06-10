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

#ifndef __FLOAT_32_RING_BUFFER_H_INCLUDED
#define __FLOAT_32_RING_BUFFER_H_INCLUDED

#include <pthread.h>

class Float32RingBuffer {

    private:
        float* buffer;
        int capacity;

        float* r_ptr;
        int r_avail;
        int r_buffer_avail;

        float* w_ptr;
        int w_avail;
        int w_buffer_avail;

        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    private:
        void _read(float* dest, int count);
        void _write(float* source, int count);
    
    public:
        Float32RingBuffer(int capacity);

        int r_buf_avail();
        int read(float* dest, int count);

        int w_buf_avail();
        int write(float* source, int count);        

        void reset();
};

#endif
