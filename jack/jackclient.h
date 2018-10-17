#ifndef JACKCLIENT_H
#define JACKCLIENT_H

#include <jack/jack.h>

#include <QObject>
#include <QList>

#include "jackfadermodule.h"

class JackModule;
class JackPort;

enum JackPortType {
    INPUT_PORT,
    OUTPUT_PORT
};

class JackClient : public QObject
{
    Q_OBJECT

private:
    jack_client_t* client;
    QList<JackModule*> modules;
    bool can_process = false;
    JackFaderModule* micFaderModule;
    void micLevelChanged(int value);

public:
    explicit JackClient(const char* const application_name);
    ~JackClient();
    void close();
    int process_callback(jack_nframes_t nframes);
    JackPort* register_port(QString name, JackPortFlags jack_port_flags);
    JackPort* get_system_port(QString name);
    void connectPorts(JackPort* source, JackPort* destination);
    void process();
    void register_module(JackModule* module);
    jack_nframes_t getSampleRate();
};

#endif // JACKCLIENT_H
