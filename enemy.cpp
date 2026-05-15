#include "enemy.h"

Enemy::Enemy(QString img_path, QGraphicsScene* scene
             ,int spawnPoint_x, int spawnPoint_y, int level):GameObject(img_path,scene)
{
    spaw_x = spawnPoint_x; spaw_y = spawnPoint_y;
    if(level == 1){
        type = rand()%2;
    }else if(level == 2){
        type = rand()%4;
    }
}
