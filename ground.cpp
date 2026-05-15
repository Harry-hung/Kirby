#include "ground.h"
#include <QString>

Ground::Ground(QString path, QGraphicsScene *scene, int x, int y):GameObject(path,scene)
{
    setPos(x,y);
}
