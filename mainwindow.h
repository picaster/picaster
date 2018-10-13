#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QPushButton>

#include "mediafile.h"
#include "optionsdialog.h"
#include "buttons/trackbutton.h"

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
    QPushButton* jingleButtons[6];
    OptionsDialog* optionsDialog;

    int tracksPlaying = 0;
    int jackLocks = 0;
    JackClient* jackClient;

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
    void jackStateChanged(bool checked);
    void jingleButtonClicked();
    void recordButtonClicked();
    void streamButtonClicked();
    void micLevelChanged(int value);
    void exitActionTriggered();
    void optionsActionTriggered();
};

#endif // MAINWINDOW_H
