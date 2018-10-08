#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <QObjectUserData>

class MediaFile : public QObjectUserData
{
private:
    QString _filePath;
    QString _artist;
    QString _title;
    int _duration;
    bool _playing;

public:
    MediaFile(QString const filePath, const char* const artist, const char* const title, int duration);
    inline QString const filePath() { return _filePath; }
    inline QString const artist() { return _artist; }
    inline QString const title() { return _title; }
    inline int duration() { return _duration; }
    inline bool playing() { return _playing; }
    inline void setPlaying(bool playing) { _playing = playing; }
};

#endif // MEDIAFILE_H
