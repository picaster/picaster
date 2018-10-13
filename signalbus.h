#ifndef SIGNALBUS_H
#define SIGNALBUS_H

#include <QObject>

class SignalBus : public QObject
{
    Q_OBJECT

public:
    static SignalBus* instance;

private:
    SignalBus();

signals:
    void jackStateChanged(bool connected);
    void micStateChanged(bool open);
    void trackStarted();
    void trackStopped();
};

#endif // SIGNALBUS_H
