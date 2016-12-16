#ifndef DIAGRAMMWIDGET_H
#define DIAGRAMMWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QFont>

class diagrammWidget : public QWidget
{
    Q_OBJECT
public:
    explicit diagrammWidget(QWidget *parent = 0);
    diagrammWidget(QString title, int value);
    void paintEvent(QPaintEvent* event);

    void setTitle(QString title);
    QString getTitle();

    void setValue(int value);
    int getValue();

private:
    QString m_title;
    int m_value;

signals:

public slots:
};

#endif // DIAGRAMMWIDGET_H
