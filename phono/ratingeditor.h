#ifndef RATINGEDITOR_H
#define RATINGEDITOR_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include "item.h"

class RatingEditor : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public:
    explicit RatingEditor(QWidget *parent = 0);
    QSize sizeHint() const;

    void setRate(int rating);
    int rate();
    int rateAt(int pos);

signals:
    void editingFinished();

private:
    int m_rate;
};

#endif // RATINGEDITOR_H
