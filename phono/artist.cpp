#include "artist.h"

Artist::Artist():Item()
{

}

void Artist::setCountry(QString country)
{
    m_country = country;
}

void Artist::setPhoto(QPixmap photo)
{
    m_photo = photo;
}

Artist* Artist::toArtist()
{
    return this;
}

Album *Artist::toAlbum()
{
    return NULL;
}

Song *Artist::toSong()
{
    return NULL;
}

QDataStream& operator << (QDataStream &stream, Artist &artist)
{
    stream << artist.m_name << artist.m_comment << artist.m_photo << artist.m_country;
    int count = artist.childCount();
    stream << count;

    for(int i=0; i<count; ++i)
    {
        Album *album = artist.childAt(i)->toAlbum();
        if(album)
            stream << *album;
    }

    return stream;
}

QDataStream& operator >> (QDataStream &stream, Artist &artist)
{
    stream >> artist.m_name >> artist.m_comment >> artist.m_photo >> artist.m_country;
    return stream;
}
