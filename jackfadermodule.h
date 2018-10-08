#ifndef JACKSLIDERMODULE_H
#define JACKSLIDERMODULE_H

#include <QString>

#include "jackmodule.h"

class JackPort;

class JackFaderModule : public JackModule
{
private:
    JackPort* mic_in_l;
    JackPort* mic_in_r;
    JackPort* mic_out_l;
    JackPort* mic_out_r;
    bool muted = true;
    long double amplification = 1.0l;

public:
    JackFaderModule(QString prefix, JackClient* jack_client);
    void process(const jack_nframes_t nframes) const;
    void mute();
    void unmute();
    void set_amplification(long double amplification);
};

#endif // JACKSLIDERMODULE_H
