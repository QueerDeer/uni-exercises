#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include <QGraphicsPathItem>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPoint>

class PuzzlePiece : public QGraphicsPathItem
{  
public:
    enum ConnectorPosition {None,In,Out,};
    enum Diretcion{North, East, South, West};
    PuzzlePiece(ConnectorPosition north,ConnectorPosition east, ConnectorPosition south, ConnectorPosition west);

    ConnectorPosition C_position[4];
    float M_size;
    PuzzlePiece* M_neighbours[4];
    int count;

    void setCount(QSize s);
    QPoint coordinates();
    void setCoordinates(QPoint p);
    QPixmap Pixmap();
    void setPixmap(QPixmap p);
    void link(PuzzlePiece* element, Diretcion dir);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void checkNeighbours(QVector<QPoint> &checked);
    void findneighbour(Diretcion dir);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    constructShape(QPainterPath& p);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPoint Coordinates;
    QPixmap M_pixmap;
};

#endif // PUZZLEPIECE_H
