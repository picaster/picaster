#include "jackfadermodule.h"
#include "jackclient.h"
#include "jackport.h"

JackFaderModule::JackFaderModule(QString prefix, JackClient* jack_client) : JackModule()
{
    mic_in_l = jack_client->register_port(prefix + "_in_l", JackPortIsInput);
    mic_in_r = jack_client->register_port(prefix + "_in_r", JackPortIsInput);
    mic_out_l = jack_client->register_port(prefix + "_out_l", JackPortIsOutput);
    mic_out_r = jack_client->register_port(prefix + "_out_r", JackPortIsOutput);

    auto playback_1 = jack_client->get_system_port("playback_1");
    auto playback_2 = jack_client->get_system_port("playback_2");
    auto capture_1 = jack_client->get_system_port("capture_1");
    auto capture_2 = jack_client->get_system_port("capture_2");

    jack_client->connect(capture_1, mic_in_l);
    jack_client->connect(capture_2, mic_in_r);
    jack_client->connect(mic_out_l, playback_1);
    jack_client->connect(mic_out_r, playback_2);

    jack_client->register_module(this);
}

void
JackFaderModule::process(const jack_nframes_t nframes) const
{
    auto mic_amplification = muted ? 0.0l : this->amplification;

    auto mic_in_l_buffer = mic_in_l->get_buffer(nframes);
    auto mic_in_r_buffer = mic_in_r->get_buffer(nframes);
    auto mic_out_l_buffer = mic_out_l->get_buffer(nframes);
    auto mic_out_r_buffer = mic_out_r->get_buffer(nframes);

    for (jack_nframes_t frame = 0; frame < nframes; frame++)
    {
        mic_out_l_buffer[frame] = static_cast<float>(mic_amplification) * mic_in_l_buffer[frame];
        mic_out_r_buffer[frame] = static_cast<float>(mic_amplification) * mic_in_r_buffer[frame];
    }
}

void
JackFaderModule::mute()
{
    muted = true;
}

void
JackFaderModule::unmute()
{
    muted = false;
}

void
JackFaderModule::set_amplification(long double amplification)
{
    this->amplification = amplification;
}
