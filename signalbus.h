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
    void micLevelChanged(int value);

    void streamStateChanged(bool streaming);
    void recordStateChanged(bool recording);
    void trackStarted();
    void trackStopped();
};

#endif // SIGNALBUS_H
