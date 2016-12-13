#include "musicmodel.h"

musicmodel::musicmodel()
{
    m_root = new Item();
}

musicmodel::~musicmodel()
{
    if(m_root)
        delete m_root;
    m_root=0;
}

QModelIndex musicmodel::index(int row, int column, const QModelIndex &parent) const
{
    Item* parentItem=m_root;
    if (parent.isValid())
        parentItem=static_cast<Item*>(parent.internalPointer());
    if (parentItem->m_children.value(row))
        return createIndex(row, column, parentItem->childAt(row));
    else
        return QModelIndex();
}

QModelIndex musicmodel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
    {
        return QModelIndex();
    }
    Item *item = static_cast<Item*>(child.internalPointer());
    if(item == m_root)
        return QModelIndex();
    Item *parentItem = item->m_parent;
    if(parentItem == m_root)
        return QModelIndex();
    Item *grandparentItem = parentItem->m_parent;
    if(!grandparentItem)
        return QModelIndex();
    else
        return createIndex(grandparentItem->indexOf(parentItem), 0, (void*)parentItem);
}

int musicmodel::columnCount(const QModelIndex &parent) const
{

       return 4;

}

int musicmodel::rowCount(const QModelIndex &parent) const
{
    Item* parentItem=m_root;
    if(parent.isValid())
        parentItem=static_cast<Item*>(parent.internalPointer());
    return parentItem->childCount();
}

QVariant musicmodel::data(const QModelIndex &index, int role) const
{
    Item *item=m_root;
    if(index.isValid())
        item=static_cast<Item*>(index.internalPointer());
    else return QVariant();

    switch(role)
    {
    case Qt::DisplayRole:
        switch(index.column())
        {
        case 0:
            return item->m_name;
            break;

        case 1:
            if(item->toSong()){
                return item->toSong()-> m_time;}
            return "";

            break;
        case 2:
            if(item->toSong()){
                return item->toSong()->m_rating;}
            return "";

            break;
        case 3:
            if(item->toSong()){
                return  item->toSong()->m_comment;}
            return "";
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
    return QVariant();
}

Qt::ItemFlags musicmodel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
             return 0;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool musicmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && (role == Qt::EditRole || role ==Qt::DisplayRole))
    {
        if(index.column() == 0)
            static_cast<Item*>(index.internalPointer())->setName(value.toString());

        if(static_cast<Song*>(index.internalPointer())->toSong())
        {

            if(index.column() == 1)
                static_cast<Song*>(index.internalPointer())->setTime(value.toString());
            if(index.column() == 2)

                static_cast<Song*>(index.internalPointer())->setRating(value.toInt());
            if(index.column() == 3)
                static_cast<Song*>(index.internalPointer())->setComment(value.toString());
        }

        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool musicmodel::insertRows(int row, int count, const QModelIndex &parent)
{
    Item *parentItem;

    if(parent.isValid())
        parentItem = static_cast<Item*>(parent.internalPointer());
    else parentItem=m_root;

    for ( int i = row; i <= (row + count - 1); i++ )
    {
        Item* item;
        if(parentItem==m_root)
            item = new Artist();
        if(parentItem->toArtist())
            item = new Album();
        if(parentItem->toAlbum())
            item = new Song();
        beginInsertRows(parent, row, row+count-1);
        parentItem->insertChild(item, i);
        endInsertRows();
    }

    return true;
}

bool musicmodel::removeRows(int row, int count, const QModelIndex &parent)
{
    Item *parentItem;
    if(parent.isValid())
        parentItem = static_cast<Item*>(parent.internalPointer());
    else parentItem=m_root;

    for ( int i = (row + count - 1); i >= row; i-- )
    {
        QModelIndex child = index(i, 0, parent);
        removeRows(0, rowCount(child), child);
        beginRemoveRows(parent, 0, 0);
        parentItem->takeChild(i);
        endRemoveRows();
    }

    return true;
}

QVariant musicmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
    {
        switch(section)
        {
        case 0:
            return QObject::tr("Track name");

        case 1:
            return QObject::tr("Time mm:ss:ms");

        case 2:
            return QObject::tr("Rating n/5");

        case 3:
            return QObject::tr("Comment supplement");

        default:
            break;
        }
    }
    return QVariant();
}
