#ifndef JACKMODULE_H
#define JACKMODULE_H

#include <QString>

#include <jack/jack.h>

class JackClient;

class JackModule
{
public:
    JackModule();
    virtual void process(const jack_nframes_t nframes) const;
    virtual ~JackModule();
};

#endif // JACKMODULE_H
