#ifndef BASEBUTTON_H
#define BASEBUTTON_H

#include <QPushButton>

#include "../mediafile.h"

class BaseButton : public QPushButton
{
private:
    MediaFile* mediaFile = nullptr;
    bool jackStarted = false;

protected:
    bool canPlay();

public:
    explicit BaseButton(QWidget *parent = nullptr);
    ~BaseButton();
    void setMediaFile(MediaFile* mediaFile);
    MediaFile* getMediaFile();
};

#endif // BASEBUTTON_H
