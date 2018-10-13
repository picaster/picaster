#include "basebutton.h"
#include "utils/signalbus.h"

BaseButton::BaseButton(QWidget *parent) : QPushButton(parent)
{
    connect(SignalBus::instance, &SignalBus::jackStateChanged, [=](bool started) { this->jackStarted = started; });
}

BaseButton::~BaseButton()
{

}

void
BaseButton::setMediaFile(MediaFile* mediaFile)
{
    this->mediaFile = mediaFile;
}

MediaFile*
BaseButton::getMediaFile()
{
    return this->mediaFile;
}

bool
BaseButton::canPlay()
{
    return this->jackStarted;
}
