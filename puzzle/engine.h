#ifndef ENGINE_H
#define ENGINE_H

#include<QPixmap>
#include<QSize>
#include<QGraphicsScene>
#include"puzzlepiece.h"

class Engine : public QGraphicsScene
{
public:
    Engine();
    PuzzlePiece::ConnectorPosition reverse(PuzzlePiece::ConnectorPosition pos);
    void setup(QSize size, QPixmap p);

};

#endif // ENGINE_H
