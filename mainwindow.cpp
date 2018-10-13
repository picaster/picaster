#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QSettings>
#include <taglib.h>
#include <taglib/fileref.h>
#include <jack/jack.h>

#include <iostream>
#include <math.h>

#include "jack/jackclient.h"
#include "jack/jackfadermodule.h"
#include "micbutton.h"
#include "signalbus.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), optionsDialog(new OptionsDialog(this))
{
    ui->setupUi(this);
}

void
MainWindow::exitActionTriggered()
{
    QApplication::quit();
}

void
MainWindow::optionsActionTriggered()
{
    if (optionsDialog->exec() == QDialog::Accepted) {
        QSettings settings;
        settings.setValue("input device", optionsDialog->getInputDevice());
        settings.setValue("output device", optionsDialog->getOutputDevice());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    connect(SignalBus::instance, &SignalBus::jackStateChanged, this, &MainWindow::jackStateChanged);

    ui->recordButton->setEnabled(false);
    connect(ui->recordButton, SIGNAL(clicked()), this, SLOT(recordButtonClicked()));

    ui->streamButton->setEnabled(false);
    connect(ui->streamButton, SIGNAL(clicked()), this, SLOT(streamButtonClicked()));

    connect(ui->micLevelSlider, SIGNAL(valueChanged(int)), this, SLOT(micLevelChanged(int)));

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitActionTriggered()));
    connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(optionsActionTriggered()));
}

void MainWindow::setButtonText(QPushButton* button, MediaFile* mediaFile)
{
    QString label = mediaFile->artist() + " - " + mediaFile->title();
    int seconds = mediaFile->duration() % 60;
    int minutes = (mediaFile->duration() - seconds) / 60;
    button->setText(QString("%1\n%2:%3").arg(label).arg(QString::number(minutes)).arg(seconds, 2, 10, QLatin1Char('0')));
}

void MainWindow::trackButtonShiftClicked(QPushButton* button)
{
    if (button->isChecked()) button->toggle();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open media file"), QString(), tr("Audio files (*.flac *.mp3)"));
    if (!fileName.isEmpty())
    {
        TagLib::FileRef f(fileName.toUtf8().data());
        TagLib::String artist = f.tag()->artist();
        TagLib::String title = f.tag()->title();
        int duration = f.audioProperties()->length();
        MediaFile* mediaFile = new MediaFile(fileName, artist.toCString(), title.toCString(), duration);
        button->setUserData(0, mediaFile);
        setButtonText(button, mediaFile);
    }
}

void
MainWindow::micLevelChanged(int value)
{
    long double f_value = value / 100.0l;
    long double db_value = 65 * log10(f_value);
    /*micFaderModule->set_amplification(pow(10, db_value / 20.0l));*/
}

void
MainWindow::jingleButtonClicked()
{
    Qt::KeyboardModifiers kn = QGuiApplication::keyboardModifiers();
    QPushButton* button = dynamic_cast<QPushButton*>(sender());

    if (button->isChecked() && !ui->jackButton->isChecked()) {
        button->toggle();
        return;
    }

    manageJackButton(button);

    if (kn & Qt::KeyboardModifier::ShiftModifier)
    {
        //std::cerr << "Shift clicked " << button->objectName().toUtf8().data() << std::endl;
    }
    else if (kn & Qt::KeyboardModifier::ControlModifier)
    {
        //std::cerr << "Control clicked " << button->objectName().toUtf8().data() << std::endl;
    }
    else
    {
        //std::cerr << "Clicked " << button->objectName().toUtf8().data() << std::endl;
    }
}

void
MainWindow::manageJackButton(QPushButton* button)
{
    if (button->isChecked())
    {
        ui->jackButton->setDisabled(true);
        jackLocks += 1;
    }
    else
    {
        jackLocks -= 1;
        if (jackLocks == 0)
        {
            ui->jackButton->setDisabled(false);
        }
    }
}

bool
MainWindow::isMicOpen()
{
    return ui->micButton->isChecked();
}
void
MainWindow::recordButtonClicked()
{
    manageJackButton(ui->recordButton);
}

void
MainWindow::streamButtonClicked()
{
    manageJackButton(ui->streamButton);
}

void
MainWindow::jackStateChanged(bool checked)
{
    if (checked)
    {
        jackClient = new JackClient("PiCasterQt");
        jackClient->process();
    }
    else
    {
        jackClient->close();
        delete jackClient;
    }
}
