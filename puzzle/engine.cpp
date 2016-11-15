#include<QVector>
#include<QRect>
#include<QTime>
#include "engine.h"

Engine::Engine()
{

}

PuzzlePiece::ConnectorPosition Engine::reverse(PuzzlePiece::ConnectorPosition pos)
{

    switch(pos)
    {
        case PuzzlePiece::None: return PuzzlePiece::None;
        case PuzzlePiece::In: return PuzzlePiece::Out;
        case PuzzlePiece::Out: return PuzzlePiece::In;
    }
    return PuzzlePiece::None;
}

void Engine::setup(QSize size, QPixmap p)
{
    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    this->clear();
    PuzzlePiece::ConnectorPosition storedWest;
    QPixmap picture=p.scaled(69*size.width(), 69*size.height());
    QVector<PuzzlePiece::ConnectorPosition> prev(size.width(), PuzzlePiece::None);
    for (int row=0;row<size.height();row++)
        for(int col=0; col<size.width();col++)
        {
            PuzzlePiece::ConnectorPosition curr[4];
            curr[0]=reverse(prev[col]);
            curr[1] = qrand() % 2 ? PuzzlePiece::In : PuzzlePiece::Out;
            curr[2] = qrand() % 2 ? PuzzlePiece::In : PuzzlePiece::Out;
            curr[3] = reverse(storedWest);
            if (col==size.width()-1)
                curr[1]=PuzzlePiece::None;
            if (row==size.height()-1)
                curr[2]=PuzzlePiece::None;

            PuzzlePiece* piece = new PuzzlePiece(curr[0], curr[1], curr[2], curr[3]);

            storedWest=curr[1];
            prev[col]=curr[2];

            piece->setFlag(QGraphicsItem::ItemIsMovable);
            piece->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
            piece->setPos(qrand()%500, qrand()%500);

            QRect rect=piece->boundingRect().toRect();
            rect.translate(0.5*69+col*69, 0.5*69+row*69);
            QPixmap pict=picture.copy(rect);
            piece->setPixmap(pict);
            piece->setCoordinates(QPoint(col, row));
            piece->setCount(size);
            addItem(piece);
        }
}
