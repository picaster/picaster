#include <alsa/asoundlib.h>

#include <QSettings>

#include <stdio.h>
#include <iostream>

#include "optionsdialog.h"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    QString sName;
    QString sCurName;
    int iCurCard = -1;
    int iCards = 0;

    snd_ctl_t* handle;
    snd_ctl_card_info_t *info;
    snd_pcm_info_t *pcminfo;
    snd_ctl_card_info_alloca(&info);
    snd_pcm_info_alloca(&pcminfo);
    const QString sPrefix("hw:%1");
    const QString sSuffix(" (%1)");
    const QString sSubSuffix("%1,%2");
    QString sName2, sSubName2;
    bool bCapture, bPlayback;
    int iCard = -1;

    QSettings settings;
    QString sCurrentInputDevice = settings.value("input device").toString();
    QString sCurrentOutputDevice = settings.value("output device").toString();

    while (snd_card_next(&iCard) >= 0 && iCard >= 0) {
        sName = sPrefix.arg(iCard);
        if (snd_ctl_open(&handle, sName.toUtf8().constData(), 0) >= 0 && snd_ctl_card_info(handle, info) >= 0) {
            sName2 = sPrefix.arg(snd_ctl_card_info_get_id(info));

            QListWidgetItem* item = addCard(sName2, snd_ctl_card_info_get_name(info) + sSuffix.arg(sName), ui->inputDeviceList);
            if (sName2 == sCurrentInputDevice) {
                item->setSelected(true);
            }

            item = addCard(sName2, snd_ctl_card_info_get_name(info) + sSuffix.arg(sName), ui->outputDeviceList);
            if (sName2 == sCurrentOutputDevice) {
                item->setSelected(true);
            }

            if (sCurName == sName || sCurName == sName2) iCurCard = iCards;
            ++iCards;
            int iDevice = -1;
            while (snd_ctl_pcm_next_device(handle, &iDevice) >= 0 && iDevice >= 0) {
                // Capture devices..
                bCapture = false;
                {
                    snd_pcm_info_set_device(pcminfo, iDevice);
                    snd_pcm_info_set_subdevice(pcminfo, 0);
                    snd_pcm_info_set_stream(pcminfo, SND_PCM_STREAM_CAPTURE);
                    bCapture = (snd_ctl_pcm_info(handle, pcminfo) >= 0);
                }
                bPlayback = false;
                {
                    snd_pcm_info_set_device(pcminfo, iDevice);
                    snd_pcm_info_set_subdevice(pcminfo, 0);
                    snd_pcm_info_set_stream(pcminfo, SND_PCM_STREAM_PLAYBACK);
                    bPlayback = (snd_ctl_pcm_info(handle, pcminfo) >= 0);
                }

                QString sSubName  = sSubSuffix.arg(sName).arg(iDevice);
                QString sSubName2 = sSubSuffix.arg(sName2).arg(iDevice);
                if (bCapture) {
                    QListWidgetItem* item = addCard(sSubName2, snd_ctl_card_info_get_name(info) + sSuffix.arg(sName), ui->inputDeviceList);
                    if (sSubName2 == sCurrentInputDevice) {
                        item->setSelected(true);
                    }
                }
                if (bPlayback) {
                    QListWidgetItem* item = addCard(sSubName2, snd_ctl_card_info_get_name(info) + sSuffix.arg(sName), ui->outputDeviceList);
                    if (sSubName2 == sCurrentOutputDevice) {
                        item->setSelected(true);
                    }
                }
                if (sCurName == sSubName || sCurName == sSubName2) iCurCard = iCards;
                ++iCards;
            }
        }
        snd_ctl_close(handle);
    }
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

QListWidgetItem*
OptionsDialog::addCard(const QString& sName, const QString& sDescription, QListWidget* list)
{
    QListWidgetItem* item = new QListWidgetItem(sName);
    list->addItem(item);
    return item;
}

QString
OptionsDialog::getInputDevice()
{
    QListWidgetItem* item = ui->inputDeviceList->selectedItems().first();
    return item->text();
}

QString
OptionsDialog::getOutputDevice()
{
    QListWidgetItem* item = ui->outputDeviceList->selectedItems().first();
    return item->text();
}
