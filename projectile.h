#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "gameobject.h"

class Projectile : public GameObject
{
public:
    Projectile(QString path, QGraphicsScene* scene);
};

#endif // PROJECTILE_H
