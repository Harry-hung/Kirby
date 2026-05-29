#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "gameobject.h"
#include "enemy.h"
#include "kirby.h"
#include <cmath>
class Projectile : public GameObject
{
public:
    Projectile(QGraphicsScene* scene,
               double spaw_x,double spaw_y,QString target_name, int facing, QString type="Fireball");
    void update();
    bool hit();
    void self_destroy();

private:
    double spaw_x=0, spaw_y=0;
    QString target ="";// Player/Enemy
    QString type ="Fireball";//Projectile type Star/Fireball/Gas
    QGraphicsScene* scene=nullptr;
    int frame=0;

    void Star();
    void Fireball();
    void Gas();

    enum dir{
        right, left
    };

    int facing = right;

    //碰撞箱
    QPainterPath shape() const override;
};

#endif // PROJECTILE_H
