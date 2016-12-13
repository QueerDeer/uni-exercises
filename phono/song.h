#ifndef SONG_H
#define SONG_H
#include "item.h"
#include <QTime>

class Song : public Item
{
public:
    Song(); //
    void setRating(int rating); //
    void setTime(QString time); //

    Artist* toArtist(); //
    Album* toAlbum(); //
    Song* toSong(); //

    int m_rating;
    QTime m_time;
};

#endif // SONG_H
