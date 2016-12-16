#include "diagrammwidget.h"

diagrammWidget::diagrammWidget(QWidget *parent) : QWidget(parent)
{

}

void diagrammWidget::setTitle(QString title)
{
    m_title = title;
}

QString diagrammWidget::getTitle()
{
    return m_title;
}

void diagrammWidget::setValue(int value)
{
    m_value = value;
}

int diagrammWidget::getValue()
{
    return m_value;
}

diagrammWidget::diagrammWidget(QString title, int value)
{
    setTitle(title);
    setValue(value);
}

void diagrammWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPen pen(Qt::darkBlue);
    painter.setPen(pen);

    QFont font = painter.font();
    font.setPointSize(16);
    font.setWeight(QFont::DemiBold);
    painter.setFont(font);
    painter.drawText(rect().center().x()-128, 16, getTitle());

    QRect rectangle(rect().x(), rect().y()+32, rect().width()-100, rect().height()-100);
    painter.drawEllipse(rectangle);
    painter.setBrush(Qt::darkBlue);
    painter.drawPie(rectangle, 1440, 57.60*getValue());
}
