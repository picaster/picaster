#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include "utils/mediafile.h"

class MediaPlayer
{
private:
    unsigned int sampleRate;

public:
    MediaPlayer(unsigned int sampleRate);
    void start();
    void stop();
    void stopAll();
};

#endif // MEDIAPLAYER_H
