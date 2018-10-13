#ifndef JACKMODULE_H
#define JACKMODULE_H

#include <QObject>
#include <QString>

#include <jack/jack.h>

class JackClient;

class JackModule : public QObject
{
    Q_OBJECT

public:
    JackModule();
    virtual void process(const jack_nframes_t nframes) const;
    virtual ~JackModule();
};

#endif // JACKMODULE_H
