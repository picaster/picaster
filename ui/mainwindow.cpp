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
#include "utils/signalbus.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), optionsDialog(new OptionsDialog(this))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    connect(SignalBus::instance, &SignalBus::jackStateChanged, this, &MainWindow::jackStateChanged);
    connect(SignalBus::instance, &SignalBus::trackStarted, this, &MainWindow::trackStarted);
    connect(SignalBus::instance, &SignalBus::trackStopped, this, &MainWindow::trackStopped);

    connect(ui->actionExit, &QAction::triggered, [](){ QApplication::quit(); });
    connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::optionsActionTriggered);
}

void
MainWindow::jackStateChanged(bool checked)
{
    if (checked)
    {
        jackClient = new JackClient("PiCasterQt");
        jackClient->process();
        this->mediaPlayer = new MediaPlayer(jackClient->getSampleRate());
    }
    else
    {
        this->mediaPlayer->stopAll();
        delete this->mediaPlayer;
        this->mediaPlayer = nullptr;
        jackClient->close();
        delete jackClient;
    }
}

void
MainWindow::trackStarted(MediaFile* mediaFile)
{
    std::cerr << "[MainWindow] trackStarted : " << mediaFile->filePath().toStdString() << std::endl;
}

void
MainWindow::trackStopped(MediaFile* mediaFile)
{
    std::cerr << "[MainWindow] trackStopped : " << mediaFile->filePath().toStdString() << std::endl;
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

