#ifndef PLATFORM_H
#define PLATFORM_H

#include "gameobject.h"

class Platform : public GameObject
{
public:
    Platform(QString path, QGraphicsScene *scene,int x,int y);
};

#endif // PLATFORM_H
