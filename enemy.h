#ifndef ENEMY_H
#define ENEMY_H
#include <random>
#include "gameobject.h"

class Enemy : public GameObject
{
public:
    Enemy(QString img_path, QGraphicsScene* scene,
          int spw_x, int spw_y, int level);


private:
    int spaw_x, spaw_y;

    //enemy type
    int type = 0;
    const int Waddle_Dee = 0, Gordo = 1,
            Hot_Head = 2, Sparky = 3;

    //level
    const int scene_1 = 1, scene_2 = 2;
};

#endif // ENEMY_H
