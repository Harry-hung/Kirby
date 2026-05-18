#include "door.h"

Door::Door(QString path, QGraphicsScene* scene):GameObject(path,scene)
{
    setData(0,"Door");
}
