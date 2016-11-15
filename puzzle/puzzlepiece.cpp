#include <QMessageBox>
#include <QSet>
#include <QVariant>
#include "dognail.h"
#include "dialog.h"
#include "puzzlepiece.h"
#include "engine.h"

PuzzlePiece::PuzzlePiece(ConnectorPosition north, ConnectorPosition east, ConnectorPosition south, ConnectorPosition west)
{
    C_position[North]=north;
    C_position[East]=east;
    C_position[South]=south;
    C_position[West]=west;
    M_size=69;
    M_neighbours[0]=0;
    M_neighbours[1]=0;
    M_neighbours[2]=0;
    M_neighbours[3]=0;
    QPainterPath a;
    constructShape(a);
    setPath(a);
}

PuzzlePiece::constructShape (QPainterPath &p)
{
    nail=1;
    p.moveTo(-1*M_size/2., -1*M_size/2.);
    p.lineTo(-1*M_size/4., -1*M_size/2.);
    C_position[North]==Out ? p.arcTo(-1*M_size/4., -3*M_size/4., M_size/2., M_size/2., 180., -180.):
    C_position[North]==In ? p.arcTo(-1*M_size/4., -3*M_size/4., M_size/2., M_size/2., 180., 180.):
    p.lineTo(M_size/4., -1*M_size/2.);
    p.lineTo(M_size/2., -1*M_size/2.);
    p.lineTo(M_size/2., -1*M_size/4.);
    C_position[East]==Out ? p.arcTo(M_size/4., -1*M_size/4., M_size/2., M_size/2., 90., -180.):
    C_position[East]==In ? p.arcTo(M_size/4., -1*M_size/4., M_size/2., M_size/2., 90., 180.):
    p.lineTo(M_size/2., M_size/4.);
    p.lineTo(M_size/2., M_size/2.);
    p.lineTo(M_size/4., M_size/2.);
    C_position[South]==Out ? p.arcTo(-1*M_size/4., M_size/4., M_size/2., M_size/2., 0., -180.):
    C_position[South]==In ? p.arcTo(-1*M_size/4., M_size/4., M_size/2., M_size/2., 0., 180.):
    p.lineTo(-1*M_size/4., M_size/2.);
    p.lineTo(-1*M_size/2., M_size/2.);
    p.lineTo(-1*M_size/2., M_size/4.);
    C_position[West]==Out ? p.arcTo(-3*M_size/4., -1*M_size/4., M_size/2., M_size/2., 270., -180.):
    C_position[West]==In ? p.arcTo(-3*M_size/4., -1*M_size/4., M_size/2., M_size/2., 270., 180.):
    p.lineTo(-1*M_size/2., -1*M_size/4.);
    p.lineTo(-1*M_size/2., -1*M_size/2.);
    p.closeSubpath();
}

void PuzzlePiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipPath(path());
    QRect rect = boundingRect().toRect();
    painter->drawPixmap(rect.x(), rect.y(), Pixmap());
    painter->drawPath(path());
}

QPixmap PuzzlePiece::Pixmap()
{
    return M_pixmap;
}
void PuzzlePiece::setPixmap(QPixmap p)
{
    M_pixmap=p;
    update();
}
QPoint PuzzlePiece::coordinates()
{
    return Coordinates;
}

void PuzzlePiece::setCoordinates(QPoint p)
{
    Coordinates.setX(p.x());
    Coordinates.setY(p.y());
}

void PuzzlePiece::setCount(QSize s)
{
    count=s.width()*s.height();
}

void PuzzlePiece::link(PuzzlePiece *element, PuzzlePiece::Diretcion dir)
{

    this->M_neighbours[dir] = element;
    if(dir==North)
    {
        element->M_neighbours[2]=this;
    }
    if(dir==East)
    {
        element->M_neighbours[3]=this;
    }
    if(dir==South)
    {
        element->M_neighbours[0]=this;
    }
    if(dir==West)
    {
        element->M_neighbours[1]=this;
    }

}

void PuzzlePiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QVector<QPoint> check;
    checkNeighbours(check);
    if(nail*check.count()==count)
    {
        QMessageBox::information(0,"Challenge","Completed");
        nail=0;
        Dialog * dialog = new Dialog;
        dialog->show();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void PuzzlePiece::checkNeighbours(QVector<QPoint> &checked)
{
    if(checked.contains(coordinates()))
        return;
    checked.insert(checked.begin(), coordinates());
    findneighbour(North);
    findneighbour(East);
    findneighbour(South);
    findneighbour(West);

    if (M_neighbours[North])
        M_neighbours[North]->checkNeighbours(checked);
    if (M_neighbours[East])
        M_neighbours[East]->checkNeighbours(checked);
    if (M_neighbours[South])
        M_neighbours[South]->checkNeighbours(checked);
    if (M_neighbours[West])
        M_neighbours[West]->checkNeighbours(checked);

}
void PuzzlePiece::findneighbour(PuzzlePiece::Diretcion dir)
{
    PuzzlePiece* element;
    if (M_neighbours[dir])
        return;
    if (dir==North)
    {
        element=(PuzzlePiece*)(scene()->itemAt(pos().x(), pos().y()-69, QTransform()));
        if(element&&((element->coordinates().x()==coordinates().x())&&(element->coordinates().y()==coordinates().y()-1)))
        {
            link(element,dir);
            element->setPos(pos().x(), pos().y()-69);
        }
    }
    if (dir==South)
    {
        element=(PuzzlePiece*)scene()->itemAt(pos().x(), pos().y()+69, QTransform());
        if(element&&((element->coordinates().x()==coordinates().x())&&(element->coordinates().y()==coordinates().y()+1)))
        {
            link(element,dir);
            element->setPos(pos().x(), pos().y()+69);
        }
    }
    if (dir==East)
    {
        element=(PuzzlePiece*)scene()->itemAt(pos().x()+69, pos().y(), QTransform());
        if(element&&((element->coordinates().x()==coordinates().x()+1)&&(element->coordinates().y()==coordinates().y())))
        {
            link(element,dir);
            element->setPos(pos().x()+69, pos().y());
        }
    }
    if (dir==West)
    {
        element=(PuzzlePiece*)scene()->itemAt(pos().x()-69, pos().y(), QTransform());
        if(element&&((element->coordinates().x()==coordinates().x()-1)&&(element->coordinates().y()==coordinates().y())))
        {
            link(element,dir);
            element->setPos(pos().x()-69, pos().y());
        }
    }
}
QVariant PuzzlePiece::itemChange(QGraphicsItem::GraphicsItemChange changed, const QVariant &value)
{
    if (changed==ItemPositionHasChanged)
       {
           QPoint newPose=value.toPoint();
           if (M_neighbours[North])
               M_neighbours[North]->setPos(newPose.x(), newPose.y()-69);
           if (M_neighbours[East])
               M_neighbours[East]->setPos(newPose.x()+69, newPose.y());
           if (M_neighbours[South])
               M_neighbours[South]->setPos(newPose.x(), newPose.y()+69);
           if (M_neighbours[West])
               M_neighbours[West]->setPos(newPose.x()-69, newPose.y());

       }
    return QGraphicsItem::itemChange(changed, value);
}
