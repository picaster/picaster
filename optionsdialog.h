#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    ~OptionsDialog();
    QString getInputDevice();
    QString getOutputDevice();

private:
    Ui::OptionsDialog *ui;
    QListWidgetItem* addCard(const QString& sName, const QString& sDescription, QListWidget* list);
};

#endif // OPTIONSDIALOG_H
