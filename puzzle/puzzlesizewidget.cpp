#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include "puzzlesizewidget.h"

PuzzleSizeWidget::PuzzleSizeWidget(QWidget *parent) :QFrame(parent)
{
    setDefaultValue(QSize(3, 3));
    setDefaultMinimum(QSize(2, 2));
    setDefaultMaximum(QSize(8, 8));
}

void PuzzleSizeWidget ::setValue( const QSize &s)
{
    if ( m_value == s)
        return;
    QSize old = m_value ;
    if(s.width() > m_maximum.width() || s.height() > m_maximum.height() || s.width() < m_minimum.width() || s.height() < m_minimum.height())
        return;
    m_value = s;
    emit valueChanged(s);
    if (old.width() != s.width())
        emit horizontalValueChanged(s.width());
    if (old.height() != s.height())
        emit verticalValueChanged(s.height());
    update();
}

void PuzzleSizeWidget::setPixmap(const  QPixmap &p)
{
    m_pixmap = p;
    update();
}

void PuzzleSizeWidget::setDefaultValue(const  QSize &v)
{
    m_value = v;
    update();
}

void PuzzleSizeWidget::setDefaultMinimum(const  QSize &min)
{
    m_minimum = min;
    update();
}

void PuzzleSizeWidget::setDefaultMaximum(const  QSize &max)
{
    m_maximum = max;
    update();
}

void PuzzleSizeWidget::paintEvent(QPaintEvent * )
{
    QPainter painter(this);
    renderValue(&painter);
    renderGrid(&painter);
}

QSize PuzzleSizeWidget::cellSize() const
{
    return QSize(qMin(width()/maximum().width(), height()/maximum().height()),qMin(width()/maximum().width(), height()/maximum().height())).expandedTo(QApplication::globalStrut()).expandedTo(QSize(4,4));
}

void PuzzleSizeWidget::renderValue(QPainter * painter)
{
    QSize cell = cellSize();
    QSize picture(cell.width()*value().width(), cell.height()*value().height());
    if (m_pixmap.isNull())
        painter->fillRect(0, 0, picture.width(), picture.height(), QBrush(Qt::darkBlue));
    else
        painter->drawPixmap(0, 0, m_pixmap.scaled(picture));
}

void PuzzleSizeWidget::renderGrid(QPainter * painter)
{
    QSize cell = cellSize();
    painter->setBrush(Qt::NoBrush);
    for(int i = 0; i < maximum().height(); i++)
        for(int j = 0; j < maximum().width(); j++)
            painter->drawRect(j*cell.width(), i*cell.height(), cell.width(), cell.height());
}

QSize PuzzleSizeWidget::cellAt(const QPoint & pos) const
{
    return QSize(pos.x()/cellSize().width()+1, pos.y()/cellSize().height()+1);
}

void PuzzleSizeWidget::mousePressEvent(QMouseEvent *event)
{
    setValue(cellAt(event->pos()));
}

void PuzzleSizeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons()&Qt::LeftButton)
        setValue(cellAt(event->pos()));
}

QSize PuzzleSizeWidget::value() const
{
    return m_value;
}

QSize PuzzleSizeWidget::minimum() const
{
    return m_minimum;
}

QSize PuzzleSizeWidget::maximum() const
{
    return m_maximum;
}

QPixmap PuzzleSizeWidget::pixmap() const
{
    return m_pixmap;
}
