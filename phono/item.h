#ifndef ITEM_H
#define ITEM_H
#include <QList>
#include <QPixmap>

class Artist;
class Album;
class Song;

class Item
{
public:
    Item();
    ~Item();
    void insertChild(Item *, int position = -1);
    Item* takeChild(int);
    void setParent(Item *);
    Item *childAt(int) const;
    int indexOf(Item*) const;
    int childCount() const;
    void setName(QString name);
    void setComment(QString comment);

    virtual Artist* toArtist();
    virtual Album* toAlbum();
    virtual Song* toSong();

    Item *m_parent;
    QList<Item*> m_children;
    QString m_name;
    QString m_comment;
};

QDataStream& operator << (QDataStream &stream, Item &root);

QDataStream& operator <<(QDataStream &stream, Artist &artist);
QDataStream& operator >>(QDataStream &stream, Artist &artist);

QDataStream& operator <<(QDataStream &stream, Album &album);
QDataStream& operator >>(QDataStream &stream, Album &album);

QDataStream& operator <<(QDataStream &stream, Song &song);
QDataStream& operator >>(QDataStream &stream, Song &song);

#endif // ITEM_H
