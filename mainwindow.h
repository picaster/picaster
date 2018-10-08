#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QPushButton>

#include "mediafile.h"
#include "optionsdialog.h"

class JackClient;
class JackFaderModule;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QPushButton* trackButtons[8];
    QPushButton* jingleButtons[6];
    OptionsDialog* optionsDialog;

    int tracksPlaying = 0;
    int jackLocks = 0;
    JackClient* jack_client;
    JackFaderModule* mic_fader_module;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    bool isMicOpen();

private:
    void trackButtonShiftClicked(QPushButton* button);
    void setButtonText(QPushButton* button, MediaFile* mediaFile);
    void manageJackButton(QPushButton* button);

private slots:
    void trackButtonClicked();
    void jingleButtonClicked();
    void micButtonClicked();
    void jackButtonClicked();
    void recordButtonClicked();
    void streamButtonClicked();
    void micLevelChanged(int value);
    void exitActionTriggered();
    void optionsActionTriggered();

// Private utility methods    
private:
    template <typename Func>
    void forTrackButtons(Func func)
    {
        for (int i = 0; i < 8; i++)
        {
            func(this->trackButtons[i]);
        }
    }

    template <typename Func>
    void forJingleButtons(Func func)
    {
        for (int i = 0; i < 6; i++)
        {
            func(this->jingleButtons[i]);
        }
    }
};

#endif // MAINWINDOW_H
