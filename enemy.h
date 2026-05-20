#ifndef ENEMY_H
#define ENEMY_H
#include <random>
#include "gameobject.h"
#include <QList>

class Enemy : public GameObject
{
public:
    Enemy(QString img_path, QGraphicsScene* scene,
          int spw_x, int spw_y, int level,bool isGordo);
    ~Enemy();
    void update();

private:
    int spaw_x, spaw_y;

    //enemy type
    static QList<int> types;
    int type = 0;
    static const int Waddle_Dee = 0, Gordo = 1,
            Hot_Head = 2, Sparky = 3;

    void setImg();
    void movement();
    void handelCollision();

    int frame=0;
    const int img_frame = 15;
    int facing = left;
    enum facing{
        right, left
    };

    bool isBlocked();

    //level
    const int scene_1 = 1, scene_2 = 2;
    QGraphicsScene* scene=nullptr;

};

#endif // ENEMY_H
