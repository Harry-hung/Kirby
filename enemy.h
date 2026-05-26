#ifndef ENEMY_H
#define ENEMY_H
#include <random>
#include "gameobject.h"
#include <QList>

class Enemy : public GameObject
{
public:
    Enemy(QString img_path, QGraphicsScene* scene,
          int spw_x, int spw_y, int level,bool isGordo=0,int id=-1);
    ~Enemy();
    void update();
    void setDead(bool dead);
    void respawn();
    bool isDead(){return isdead;}
    QPointF getRespawn(){QPointF spawn(spaw_x,spaw_y); return spawn;}
    int getType()const{return type;}
    void Attk();//attack movement
private:
    double spaw_x, spaw_y;
    int turning_x, turning_y;
    bool isdead = false;

    double temp_x,temp_y;
    bool isYWeird();
    void handelCollisionY();

    bool isPlayerNear();

    //enemy type
    int id=-1;
    static QList<int> types;
    int type = 0;
    static const int Waddle_Dee = 0, Gordo = 1,
            Hot_Head = 2, Sparky = 3;

    void setImg();
    void movement();
    void spark_movement();
    void Turn();
    void updownMove();
    enum vertical_facing{down,up};
    int vfacing=up;

    //the distance between turning if no wall blocked
    int turning_distance=1000;

    int frame=0;
    const int img_frame = 15;
    int facing = left;
    enum facing{
        right, left
    };

    bool isWallBlocked();
    bool isSolid();
    bool isBlocked();

    //level
    int level=0;
    const int scene_1 = 1, scene_2 = 2;
    QGraphicsScene* scene=nullptr;

//For Sparky jumping
    double vy=0;
    const double gravity=1;
    const double jump_speed = -20; //the initial speed when jump
    bool isGrounded =0;

    int jump_timer=0;//for how many frames has passed since last jump
    const int jump_interval=60; //the frame between jumps

    //the attk interval
    int attk_timer=180;// 3s
};

#endif // ENEMY_H
