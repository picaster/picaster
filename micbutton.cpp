#include "micbutton.h"
#include "signalbus.h"

MicButton::MicButton(QWidget* parent) : QPushButton(parent)
{
    connect(this, &MicButton::clicked, [=]() { emit SignalBus::instance->micStateChanged(this->isChecked()); });
    connect(SignalBus::instance, &SignalBus::jackStateChanged, [=](bool isChecked) { this->setEnabled(isChecked); });
}

MicButton::~MicButton()
{

}
