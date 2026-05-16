#ifndef DOOR_H
#define DOOR_H

#include "gameobject.h"

class Door : public GameObject
{
public:
    Door(QString path, QGraphicsScene* scene);
};

#endif // DOOR_H
