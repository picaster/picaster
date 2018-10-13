#include <iostream>

#include <QGuiApplication>
#include <QFileDialog>
#include <QString>

#include <taglib.h>
#include <taglib/fileref.h>

#include "trackbutton.h"
#include "utils/mediafile.h"
#include "utils/signalbus.h"

TrackButton::TrackButton(QWidget *parent) : BaseButton(parent)
{
    connect(this, &QPushButton::clicked, this, &TrackButton::handleClick);
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
         * Do we have a file assiciated with the button ?
         */
        MediaFile* mediaFile = this->getMediaFile();
        /*
         * If not, then clicking on an "empty" button is the same as shift-clicking it.
         */
        if (mediaFile == nullptr) this->handleShiftClick();
        else
        {
            /*
             * We do have a file.
             */
            if (this->isChecked())
            {
                /*
                 * The button is checked
                 */
                if (this->canPlay())
                {
                    std::cerr << "Playing track " << mediaFile->filePath().toUtf8().data() << " for " << this->objectName().toUtf8().data() << std::endl;
                    emit SignalBus::instance->trackStarted();
                /*
                if (tracksPlaying == 2) button->toggle();
                else if (!ui->jackButton->isChecked()) button->toggle();
                else
                {
                    manageJackButton(button);
                    std::cerr << "Playing track " << mediaFile->filePath().toUtf8().data() << " for " << button->objectName().toUtf8().data() << std::endl;
                    mediaFile->setPlaying(true);
                    tracksPlaying += 1;
                    if (tracksPlaying == 2)
                    {
                        forTrackButtons([&](QPushButton* button) {
                            MediaFile* mediaFile = dynamic_cast<MediaFile*>(button->userData(0));
                            if ((mediaFile == nullptr) || (!mediaFile->playing())) button->setDisabled(true);
                        });
                    }
                }
                */
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
                emit SignalBus::instance->trackStopped();
                /*
                 * The button is unchecked : let's stop playing the file.
                 */
                /*
                manageJackButton(button);
                std::cerr << "Stopping track " << mediaFile->filePath().toUtf8().data() << " for " << button->objectName().toUtf8().data() << std::endl;
                tracksPlaying -= 1;
                mediaFile->setPlaying(false);
                setButtonText(button, mediaFile);
                if (tracksPlaying == 1)
                {
                    forTrackButtons([&](QPushButton* button) {
                        button->setDisabled(false);
                    });
                }
                */
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
