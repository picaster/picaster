#ifndef TRACKBUTTON_H
#define TRACKBUTTON_H

#include <QtWidgets/QPushButton>

#include "basebutton.h"
#include "utils/mediafile.h"

class TrackButton : public BaseButton
{
public:
    explicit TrackButton(QWidget *parent = nullptr);
    ~TrackButton();
    void init();

private:
    void handleClick();
    void handleShiftClick();
    void setLabel(MediaFile* mediaFile);
};

#endif // TRACKBUTTON_H
