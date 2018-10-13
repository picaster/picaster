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
    OptionsDialog* optionsDialog;

    JackClient* jackClient;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();

private:
    void jackStateChanged(bool checked);
    void optionsActionTriggered();
};

#endif // MAINWINDOW_H
