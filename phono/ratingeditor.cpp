#include "ratingeditor.h"

RatingEditor::RatingEditor(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);
    setAutoFillBackground(true);
}

QSize RatingEditor::sizeHint() const
{
    return QSize(100, 20);
}

void RatingEditor::setRate(int rating)
{
     m_rate = rating;
}

int RatingEditor::rate()
{
     return m_rate;
}

void RatingEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    int left_coord = 0;
    int top_coord = 0;

    if (m_rate < 0)
        m_rate = 0;

    if (m_rate > 5)
        m_rate = 5;

    for (int i=0; i<5; i++)
    {
        if (i < m_rate && m_rate > 0)
            painter.setBrush(Qt::yellow);
        else
            painter.setBrush(Qt::NoBrush);

        painter.drawEllipse(QPoint(left_coord+10, top_coord+10), 8, 8);
        left_coord+=20;
    }
}

void RatingEditor::mouseMoveEvent(QMouseEvent *event)
{
    int rate = (event->x()/20)+1;
    if (rate < 0 || rate > 5)
        return;
    else
    {
        m_rate = rate;
        update();
    }
}

void RatingEditor::mouseReleaseEvent(QMouseEvent * event)
{
    emit editingFinished();
}
