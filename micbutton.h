#ifndef MICBUTTON_H
#define MICBUTTON_H

#include <QPushButton>

class MicButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MicButton(QWidget* parent = nullptr);
    ~MicButton();
};

#endif // MICBUTTON_H
