#ifndef RATINGDELEGATE_H
#define RATINGDELEGATE_H
#include <QWidget>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QSize>
#include "artist.h"
#include "album.h"
#include "song.h"
#include "ratingeditor.h"

class ratingdelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ratingdelegate(QAbstractItemView *view);
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

public slots:
    void commitAndCloseEditor();

private:
    QAbstractItemView *m_view;
    int m_rate;
};
#endif // RATINGDELEGATE_H
