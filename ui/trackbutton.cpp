#include <iostream>

#include <QGuiApplication>
#include <QFileDialog>
#include <QString>

#include <taglib.h>
#include <taglib/fileref.h>

#include "trackbutton.h"
#include "utils/mediafile.h"
#include "utils/signalbus.h"

int TrackButton::tracksPlaying = 0;

TrackButton::TrackButton(QWidget *parent) : BaseButton(parent)
{
    connect(this, &QPushButton::clicked, this, &TrackButton::handleClick);
    connect(SignalBus::instance, &SignalBus::lockTrackButtons, [=](bool lock){
        std::cerr << "signal received : " << lock << std::endl;
        std::cerr << this->objectName().toStdString() << " : " << this->isChecked() << std::endl;
        if (this->isChecked() == false) this->setDisabled(lock);
    });
}

TrackButton::~TrackButton()
{

}

void
TrackButton::init()
{
    this->setText("Empty\n0:00");
}

void
TrackButton::handleClick()
{
    Qt::KeyboardModifiers kn = QGuiApplication::keyboardModifiers();

    if (kn & Qt::KeyboardModifier::ShiftModifier)
    {
        /*
         *  Shift was held while pressing the button.
         * Shift-clicking only makes send when the button is not checked.
         * So if it is checked now, that's ok, let's handle that.
         * If the button is not checked, then it was before, and it doesn't
         * make any sense, so let's uncheck the button and we're done.
         */
        if (this->isChecked()) this->handleShiftClick();
        else this->toggle();
    }
    else
    {
        /*
         * Do we have a file associated with the button ?
         */
        MediaFile* mediaFile = this->getMediaFile();
        if (mediaFile == nullptr) this->toggle();
        else
        {
            if (this->isChecked())
            {
                /*
                 * The button is checked
                 */
                if (this->canPlay())
                {
                    std::cerr << "Playing track " << mediaFile->filePath().toUtf8().data() << " for " << this->objectName().toUtf8().data() << std::endl;
                    emit SignalBus::instance->trackStarted(mediaFile);
                    TrackButton::tracksPlaying += 1;
                    if (TrackButton::tracksPlaying == 2)
                    {
                        emit SignalBus::instance->lockTrackButtons(true);
                    }
                }
                else
                {
                    std::cerr << "Can't play !" << std::endl;
                    this->toggle();
                }
            }
            else
            {
                std::cerr << "Stopping file" << std::endl;
                emit SignalBus::instance->trackStopped(mediaFile);
                TrackButton::tracksPlaying -= 1;
                if (TrackButton::tracksPlaying < 2)
                {
                    emit SignalBus::instance->lockTrackButtons(false);
                }
            }
        }
    }

    /*
    if (kn & Qt::KeyboardModifier::ControlModifier)
    {
        std::cerr << "Control clicked " << button->objectName().toUtf8().data() << std::endl;
    }
    else
    {
        std::cerr << "Clicked " << button->objectName().toUtf8().data() << std::endl;
    }
    */
}

void
TrackButton::handleShiftClick()
{
    if (this->isChecked()) this->toggle();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open media file"), QString(), tr("Audio files (*.flac *.mp3)"));
    if (!fileName.isEmpty())
    {
        TagLib::FileRef f(fileName.toUtf8().data());
        TagLib::String artist = f.tag()->artist();
        TagLib::String title = f.tag()->title();
        int duration = f.audioProperties()->length();
        MediaFile* mediaFile = new MediaFile(fileName, artist.toCString(), title.toCString(), duration);
        this->setMediaFile(mediaFile);
        this->setLabel(mediaFile);
    }
}

void
TrackButton::setLabel(MediaFile* mediaFile)
{
    QString label = mediaFile->artist() + " - " + mediaFile->title();
    int seconds = mediaFile->duration() % 60;
    int minutes = (mediaFile->duration() - seconds) / 60;
    this->setText(QString("%1\n%2:%3").arg(label).arg(QString::number(minutes)).arg(seconds, 2, 10, QLatin1Char('0')));
}
