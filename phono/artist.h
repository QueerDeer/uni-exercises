#ifndef ARTIST_H
#define ARTIST_H
#include "item.h"

class Artist : public Item
{
public:
    Artist();
    void setCountry(QString country);
    void setPhoto(QPixmap photo);

    Artist* toArtist();
    Album* toAlbum();
    Song* toSong();

    QPixmap m_photo;
    QString m_country;
};

#endif // ARTIST_H
