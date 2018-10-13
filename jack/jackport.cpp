#include "jackport.h"

JackPort::JackPort(jack_port_t* port)
{
    this->port = port;
}

const char*
JackPort::get_port_name() const
{
    return jack_port_name(port);
}

float*
JackPort::get_buffer(jack_nframes_t nframes)
{
    auto buffer = static_cast<float*>(jack_port_get_buffer(port, nframes));
    return buffer;
}
