#include "mediafile.h"

MediaFile::MediaFile(QString const filePath, const char* const artist, const char* const title, int duration)
{
    this->_filePath = filePath;
    this->_artist = artist;
    this->_title = title;
    this->_duration = duration;
    this->_playing = false;
}
