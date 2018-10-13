#include "buttons/streambutton.h"
#include "signalbus.h"

StreamButton::StreamButton(QWidget* parent) : QPushButton(parent)
{
    connect(this, &StreamButton::clicked, [=]() { emit SignalBus::instance->streamStateChanged(this->isChecked()); });
    connect(SignalBus::instance, &SignalBus::jackStateChanged, [=](bool started) { this->setEnabled(started); });
}
