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

#include <stdlib.h>
#include <string.h>

#include "Float32RingBuffer.h"

Float32RingBuffer::Float32RingBuffer(int capacity)
{
    this->capacity = capacity;
    this->buffer = (float*)calloc(capacity, sizeof(float));
    this->reset();
}

int
Float32RingBuffer::r_buf_avail()
{
    return this->r_avail;
}

void
Float32RingBuffer::_read(float* buffer, int count)
{
    memcpy(buffer, this->r_ptr, count * sizeof(float));
    this->r_ptr += count;
    this->r_buffer_avail -= count;
    if (this->r_buffer_avail == 0)
    {
        this->r_ptr = this->buffer;
        this->r_buffer_avail = this->capacity;
    }
}

int
Float32RingBuffer::read(float* buffer, int count)
{
    pthread_mutex_lock(&mutex);

    if (this->r_buffer_avail < count)
    {
        int tmp_count = this->r_buffer_avail;
        this->_read(buffer, tmp_count);
        this->_read(buffer + tmp_count, count - tmp_count);
    }
    else
    {
        this->_read(buffer, count);
    }
    this->r_avail -= count;
    this->w_avail += count;

    pthread_mutex_unlock(&mutex);

    return count;
}

int
Float32RingBuffer::w_buf_avail()
{
    return this->w_avail;
}

void
Float32RingBuffer::_write(float* source, int count)
{
    memcpy(this->w_ptr, source, count * sizeof(float));
    this->w_ptr += count;
    this->w_buffer_avail -= count;
    if (this->w_buffer_avail == 0)
    {
        this->w_ptr = this->buffer;
        this->w_buffer_avail = this->capacity;
    }
}

int
Float32RingBuffer::write(float* source, int count)
{
    pthread_mutex_lock(&mutex);

    if (this->w_buffer_avail < count)
    {
        int tmp_count = this->w_buffer_avail;
        this->_write(source, tmp_count);
        this->_write(source + tmp_count, count - tmp_count);
    }
    else
    {
        this->_write(source, count);
    }
    this->w_avail -= count;
    this->r_avail += count;

    pthread_mutex_unlock(&mutex);

    return count;
}

void
Float32RingBuffer::reset()
{
    this->r_ptr = this->buffer;
    this->r_avail = 0;
    this->r_buffer_avail = this->capacity;

    this->w_ptr = this->buffer;
    this->w_avail = this->capacity;
    this->w_buffer_avail = this->capacity;
}