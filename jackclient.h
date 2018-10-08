#ifndef JACKCLIENT_H
#define JACKCLIENT_H

#include <jack/jack.h>

#include <QList>

class JackModule;
class JackPort;

enum JackPortType {
    INPUT_PORT,
    OUTPUT_PORT
};

class JackClient
{
private:
    jack_client_t* client;
    QList<JackModule*> modules;
    bool can_process = false;

public:
    JackClient(const char* const application_name);
    int process_callback(jack_nframes_t nframes);
    JackPort* register_port(QString name, JackPortFlags jack_port_flags);
    JackPort* get_system_port(QString name);
    void connect(JackPort* source, JackPort* destination);
    void process();
    void register_module(JackModule* module);
};

#endif // JACKCLIENT_H
