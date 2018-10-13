#include <iostream>
#include <math.h>

#include "jackclient.h"
#include "jackport.h"
#include "jackmodule.h"
#include "utils/signalbus.h"

static int
s_process_callback(jack_nframes_t nframes, void *arg)
{
    auto jack_client = static_cast<JackClient*>(arg);
    return jack_client->process_callback(nframes);
}

JackClient::JackClient(const char* const application_name)
{
    jack_options_t options = JackNoStartServer;
    jack_status_t status;
    client = jack_client_open(application_name, options, &status);
    if (client == nullptr)
    {
        // We're in trouble
        return;
    }
    jack_set_process_callback(client, s_process_callback, this);
    jack_activate(client);

    micFaderModule = new JackFaderModule("mic", this);
    micFaderModule->mute();
    connect(SignalBus::instance, &SignalBus::micStateChanged, [=](bool isChecked) { isChecked ? micFaderModule->unmute() : micFaderModule->mute(); });
    std::cerr << "connecting signal" << std::endl;
    connect(SignalBus::instance, &SignalBus::micLevelChanged, this, &JackClient::micLevelChanged);
}

JackClient::~JackClient()
{
    if (this->micFaderModule != nullptr)
        delete this->micFaderModule;

    if (client != nullptr)
        this->close();
}

void
JackClient::close()
{
    if (this->micFaderModule != nullptr)
        delete this->micFaderModule;
    this->micFaderModule = nullptr;

    jack_client_close(client);
    client = nullptr;
}

int
JackClient::process_callback(jack_nframes_t nframes)
{
    if (!can_process) return 0;

    for (int i = 0; i < modules.size(); i++)
    {
        modules.at(i)->process(nframes);
    }

    return 0;
}

JackPort*
JackClient::register_port(QString name, JackPortFlags jack_port_flags)
{
    auto port = jack_port_register(client, name.toUtf8().data(), JACK_DEFAULT_AUDIO_TYPE, jack_port_flags, 0);
    return new JackPort(port);
}

JackPort*
JackClient::get_system_port(QString name)
{
    return new JackPort(jack_port_by_name(client, ("system:" + name).toUtf8().data()));
}

void
JackClient::connectPorts(JackPort* source, JackPort* destination)
{
    jack_connect(client, source->get_port_name(), destination->get_port_name());
}

void
JackClient::process()
{
    can_process = true;
}

void
JackClient::register_module(JackModule* module)
{
    modules.append(module);
}

void
JackClient::micLevelChanged(int value)
{
    long double f_value = value / 100.0l;
    long double db_value = 65 * log10(f_value);
    micFaderModule->setAmplification(pow(10, db_value / 20.0l));
}
