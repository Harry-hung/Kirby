#include "stages.h"

Stages::Stages(QString path, QGraphicsScene *scene,int x, int y) : GameObject(path, scene)
{

    setData(0,"Solid");
    setZValue(4);
    setPos(x,y);
}


