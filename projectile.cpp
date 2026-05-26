#include "projectile.h"

Projectile::Projectile(QString path, QGraphicsScene* scene,double spawn_x, double spawn_y):spaw_x(spawn_x),
  spaw_y(spawn_y),GameObject(path,scene)
{

}
