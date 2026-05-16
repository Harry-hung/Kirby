#include "stages.h"

Stages::Stages(QString path, QGraphicsScene *scene,int x, int y) : GameObject(path, scene)
{
    setPos(x,y);
    setZValue(4);
}
