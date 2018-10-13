#ifndef JACKPORT_H
#define JACKPORT_H

#include <jack/jack.h>

class JackPort
{
private:
    jack_port_t* port;

public:
    JackPort(jack_port_t* port);
    const char* get_port_name() const;
    float* get_buffer(jack_nframes_t nframes);
};

#endif // JACKPORT_H
