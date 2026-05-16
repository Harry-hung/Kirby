#include "platform.h"

Platform::Platform(QString path, QGraphicsScene *scene, int x, int y):GameObject(path,scene)
{
    setPos(x,y);
    setZValue(2);
}
