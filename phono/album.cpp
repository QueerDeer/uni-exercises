#include "album.h"

Album::Album():Item()
{

}

void Album::setCover(QPixmap cover)
{
    m_cover = cover;
}

void Album::setGenre(QString genre)
{
    m_genre = genre;
}

void Album::setYear(int year)
{
    m_year = year;
}

Artist *Album::toArtist()
{
    return NULL;
}

Album* Album::toAlbum()
{
    return this;
}

Song *Album::toSong()
{
    return NULL;
}

QDataStream& operator << (QDataStream &stream, Album &album)
{
    stream << album.m_name << album.m_comment<< album.m_cover << album.m_year << album.m_genre;
    int count = album.childCount();
    stream << count;

    for(int i=0; i<count; ++i)
    {
        Song *song = album.childAt(i)->toSong();
        if(song)
            stream << *song;
    }

    return stream;
}

QDataStream& operator >> (QDataStream &stream, Album &album)
{
    stream >> album.m_name >> album.m_comment >> album.m_cover >> album.m_year >> album.m_genre;
    return stream;
}
