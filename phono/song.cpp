#include "song.h"

Song::Song():Item()
{
    m_rating = 0;
}

void Song::setRating(int rating)
{
    m_rating = rating;
}

void Song::setTime(QString time)
{
    m_time = QTime::fromString(time, "HH:mm:ss");;
}

Artist *Song::toArtist()
{
    return NULL;
}

Album *Song::toAlbum()
{
    return NULL;
}

Song* Song::toSong()
{
    return this;
}

QDataStream& operator << (QDataStream &stream, Song &song)
{
    stream << song.m_name << song.m_comment << song.m_rating << song.m_time;
    return stream;
}

QDataStream& operator >> (QDataStream &stream, Song &song)
{
    stream >> song.m_name >> song.m_comment >> song.m_rating >> song.m_time;
    return stream;
}
