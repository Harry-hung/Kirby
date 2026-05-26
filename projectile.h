#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "gameobject.h"

class Projectile : public GameObject
{
public:
    Projectile(QString path, QGraphicsScene* scene,double spaw_x,double spaw_y);
    void hitWall();
    void hitTarget(QString type);//the target type player/enemy

private:
    double spaw_x, spaw_y;
};

#endif // PROJECTILE_H
