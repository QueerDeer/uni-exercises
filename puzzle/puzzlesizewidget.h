#ifndef PUZZLESIZEWIDGET_H
#define PUZZLESIZEWIDGET_H

#include <QObject>
#include <QFrame>
#include <QSize>
#include <QPixmap>


class PuzzleSizeWidget : public QFrame
{
    Q_OBJECT

public:
explicit PuzzleSizeWidget(QWidget *parent = 0);
QSize value() const;
QSize minimum() const;
QSize maximum() const;
QPixmap pixmap() const;

public slots:
void setValue(const QSize &s);
void setPixmap(const QPixmap &p);
void setDefaultValue(const QSize &v);
void setDefaultMinimum(const QSize &min);
void setDefaultMaximum(const QSize &max);

signals:
void valueChanged(const QSize &);
void horizontalValueChanged(int);
void verticalValueChanged(int);

private:
QSize m_value;
QSize m_minimum;
QSize m_maximum;
QPixmap m_pixmap;
QSize cellSize() const;
QSize cellAt(const QPoint & pos) const;

void paintEvent(QPaintEvent *);
void renderValue(QPainter * painter);
void renderGrid(QPainter * painter);
void mousePressEvent(QMouseEvent *event);
void mouseMoveEvent(QMouseEvent *event);

};

#endif // PUZZLESIZEWIDGET_H
