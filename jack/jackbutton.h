#ifndef JACKBUTTON_H
#define JACKBUTTON_H

#include <QPushButton>

#include "jack/jackclient.h"
#include "jack/jackfadermodule.h"

class JackButton : public QPushButton
{
    Q_OBJECT

private:
    int lockCount = 0;

public:
    explicit JackButton(QWidget* parent = nullptr);
    ~JackButton();
    void lock(bool lock);
};

#endif // JACKBUTTON_H
