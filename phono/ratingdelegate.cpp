#include "ratingdelegate.h"

ratingdelegate::ratingdelegate(QAbstractItemView *view)
{
    m_view = view;
}

QSize ratingdelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(100, 20);
}

void ratingdelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid() || !index.parent().isValid())
        return;

    painter->setPen(Qt::black);
    painter->setBrush(Qt::yellow);
    int m_rate = 0;
    int left_coord = option.rect.left();
    int top_coord = option.rect.top();
    Item *item = static_cast<Item*>(index.internalPointer());

    if(item->m_parent->toAlbum() )
    {
        if (index.column() == 2)
            m_rate = index.model()->data(index).toInt();

        if (m_rate < 0)
            m_rate = 0;

        if (m_rate > 5)
            m_rate = 5;

        for (int i=0; i<5; i++)
        {
            if(i == m_rate)
                painter->setBrush(Qt::NoBrush);

            painter->drawEllipse(QPoint(left_coord+10, top_coord+10), 8, 8);
            left_coord+=20;
        }
    }
}

QWidget *ratingdelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return QStyledItemDelegate::createEditor(parent, option, index);

    if(!index.parent().isValid())
        return QStyledItemDelegate::createEditor(parent, option, index);

    Item *item = static_cast<Item*>(index.internalPointer());

    if(!item->m_parent->toAlbum())
        return QStyledItemDelegate::createEditor(parent, option, index);

    if(!item->toSong())
        return QStyledItemDelegate::createEditor(parent, option, index);

    RatingEditor *editor = new RatingEditor(parent);

    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor())); //Q_OBJECT - может эксплисит и паренты допом

    return editor;

}

void ratingdelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!index.isValid())
        QStyledItemDelegate::setEditorData(editor, index);

    if(!index.parent().isValid())
        QStyledItemDelegate::setEditorData(editor, index);

    Item *item = static_cast<Item*>(index.internalPointer());

    if(!item->m_parent->toAlbum())
        QStyledItemDelegate::setEditorData(editor, index);

    if(!item->toSong())
        QStyledItemDelegate::setEditorData(editor, index);

    if(index.column() == 2)
    {
        int rating = index.data().toInt();
        RatingEditor *new_editor = qobject_cast<RatingEditor*>(editor);
        new_editor->setRate(rating);
    } else
        QStyledItemDelegate::setEditorData(editor, index);

}

void ratingdelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!index.isValid())
        QStyledItemDelegate::setModelData(editor, model, index);

    if(!index.parent().isValid())
        QStyledItemDelegate::setModelData(editor, model, index);

    Item *item = static_cast<Item*>(index.internalPointer());

    if(!item->m_parent->toAlbum())
        QStyledItemDelegate::setModelData(editor, model, index);

    if(!item->toSong())
        QStyledItemDelegate::setModelData(editor, model, index);

    if(index.column() == 2)
    {
        RatingEditor *new_editor = qobject_cast<RatingEditor*>(editor);
        item->toSong()->m_rating = new_editor->rate();
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

void ratingdelegate::commitAndCloseEditor()
{
    RatingEditor *editor = qobject_cast<RatingEditor*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
