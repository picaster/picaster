#include <iostream>

#include "jack/jackbutton.h"
#include "jack/jackclient.h"
#include "signalbus.h"

JackButton::JackButton(QWidget* parent) : QPushButton(parent)
{
    connect(this, &QPushButton::clicked, [=]() { emit SignalBus::instance->jackStateChanged(this->isChecked()); });

    connect(SignalBus::instance, &SignalBus::micStateChanged, [=](bool open) { this->lock(open); });
    connect(SignalBus::instance, &SignalBus::trackStarted, [=]() { this->lock(true); });
    connect(SignalBus::instance, &SignalBus::trackStopped, [=]() { this->lock(false); });
}

JackButton::~JackButton()
{

}

void
JackButton::lock(bool lock)
{
    if (lock) {
        this->setDisabled(true);
        this->lockCount += 1;
    }
    else
    {
        this->lockCount -= 1;
        if (this->lockCount == 0)
        {
            this->setEnabled(true);
        }
    }
}
