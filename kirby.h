#ifndef KIRBY_H
#define KIRBY_H

#include "gameobject.h"
#include "enemy.h"
#include "projectile.h"
#include <QKeyEvent>
#include <cmath>
//Special class of gameObject: player

class Kirby : public GameObject
{
public:
    Kirby(QString img_path, QGraphicsScene *scene,double spw_x, double spw_y);

    void handleCollisionX();
    void handleCollisionY();
    void handelCollideEnemy();
    void handlePressEvent(QKeyEvent *event);
    void handleReleaseEvent(QKeyEvent *event);
    void updateMovement(int speed);
    bool isNextScene();
    int y_pre_frame=0;

private:

    double spaw_x,spaw_y;
    bool isCollision();
    bool isDoor();
    bool isEnemy(Enemy** output_item=nullptr);
    int hp=5,life=3;

    void Animation();

    void moveLeft(int speed);
    void moveRight(int speed);
    void moveUp();
    void moveDown();
    void moveStop();
    void moveAttack();
    void moveHurt();
    void moveInhale();
    bool isXreleaseAfterInhale =1;
   // bool isXreleasedAfterPress=1;

    bool isGrounded = 1;
    bool isWalkable = 1;
    QGraphicsScene *scene=nullptr;
    //frame per image
    int Left_frames =0;//how many frames does Key_Left pressed
    int Right_frames = 0;//how many frames does Key_Right pressed
    int Up_frames = 0;
    int Attk_frames = 0;
    int hurt_frames = 120;
    int inhale_frames=0;
    const int image_frame = 15;//the duration of a image in frames(image per frame)

    int Down_time = 0;
    int Uptime = 0;

    //where kirby faceing left=0, right=1
    enum facing{
        left =1,
        right =0,
    };
    facing facing = right;


    bool isUp_keyPressed = 0;
    bool isDown_keyPressed = 0;
    bool isRight_keyPressed = 0;
    bool isLeft_keyPressed = 0;
    bool isXPressed =0;

    //state of kirby

    enum move{
        move_stop,
        move_ground,
        move_jump,
        move_flying,
        move_attacking,
        move_inhale,
        move_hurt
    };
    enum state{
        state_air,
        state_full, state_fire,
        state_spark, state_normal
    };

    const double kirby_fullheight=180;
    bool isAir=0;//if kirby is inflat
    state state = state_normal;
    move move = move_stop;

    const double jump_speed = -20;
    const double flying_speed = -10;
    double vy = 0; // the start jumping speed 13
    const double gravity = 1;

    //Knock back by enemy
    double vx = 0;
    const double knockup_sped = -10;
    const double knockback_sped = 10;
    const double friction = 1;

    //the highest pixel kirby can walk off
    const int maxWalkingHeight = 4;

    //previous x y
    int temp_x =0, temp_y =0;


    int frame_couter=0;
    bool isYWeird();

};

#endif // KIRBY_H
