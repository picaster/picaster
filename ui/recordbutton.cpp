#include "ui/recordbutton.h"
#include "utils/signalbus.h"

RecordButton::RecordButton(QWidget* parent) : QPushButton(parent)
{
    connect(this, &RecordButton::clicked, [=]() { emit SignalBus::instance->recordStateChanged(this->isChecked()); });
    connect(SignalBus::instance, &SignalBus::jackStateChanged, [=](bool started) { this->setEnabled(started); });
}
