#include "item.h"

Item::Item()
{
    m_parent = 0;
    m_name = QString("");
    m_comment = QString("");
}

Item::~Item()
{
    setParent(0);

    foreach (Item* child, m_children)
    {
        delete child;
    }
}

void Item::insertChild(Item *obj, int position)
{
    if (!obj) return;

    if(m_children.contains(obj))
        obj->setParent(this);
    else
    {
        m_children.insert(position, obj);
        obj->setParent(this);
    }
}

Item *Item::takeChild(int position)
{
    Item* obj = m_children.takeAt(position);

    if (obj)
        obj->setParent(0);
    return obj;
}

void Item::setParent(Item *obj)
{
    if (m_parent == obj)
        return;

    if (obj)
    {
        if(m_parent)
        {
            m_parent->takeChild(m_parent->indexOf(obj));
        }

        m_parent=obj;
        m_parent->insertChild(this);
    }
}

Item *Item::childAt(int position) const
{
    Item* obj = m_children.at(position);
    return obj;
}

int Item::indexOf(Item *obj) const
{
    return m_children.indexOf(obj, 0);
}

int Item::childCount() const
{
    return m_children.count();
}

void Item::setName(QString name)
{
    m_name=name;
}


void Item::setComment(QString comment)
{
    m_comment=comment;
}

Artist *Item::toArtist()
{
    return NULL;
}

Album *Item::toAlbum()
{
    return NULL;
}

Song *Item::toSong()
{
    return NULL;
}

QDataStream& operator << (QDataStream &stream, Item &root)
{
    int count = root.childCount();
    stream << count;

    for(int i=0; i<count; ++i)
    {
        Artist *artist = root.childAt(i)->toArtist();
        if(artist)
            stream << *artist;
    }

    return stream;
}
