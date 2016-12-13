#ifndef ALBUM_H
#define ALBUM_H
#include "item.h"


class Album : public Item
{
public:
    Album();
    void setCover(const QPixmap cover);
    void setGenre(QString genre);
    void setYear(int year);

    Artist* toArtist();
    Album* toAlbum();
    Song* toSong();

    QPixmap m_cover;
    int m_year;
    QString m_genre;
};

#endif // ALBUM_H
