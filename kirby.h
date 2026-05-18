#ifndef KIRBY_H
#define KIRBY_H

#include "gameobject.h"
#include <QKeyEvent>
//Special class of gameObject: player

class Kirby : public GameObject
{
public:
    Kirby(QString img_path, QGraphicsScene *scene);

    void handleCollisionX();
    void handleCollisionY();
    void handlePressEvent(QKeyEvent *event);
    void handleReleaseEvent(QKeyEvent *event);
    void updateMovement(int speed);


private:

    bool isCollision();
    bool isDoor();
    bool isEnemy();

    void Animation();

    void moveLeft(int speed);
    void moveRight(int speed);
    void moveUp();
    void moveDown();
    void moveStop();

    bool isGrounded = 1;
    QGraphicsScene *scene=nullptr;
    //frame per image
    int Left_frames =0;//how many frames does Key_Left pressed
    int Right_frames = 0;//how many frames does Key_Right pressed
    int Up_frames = 0;
    const int image_frame = 15;//the duration of a image in frames(image per frame)

    int Down_time = 0;
    int Uptime = 0;

    //where kirby faceing left=0, right=1
    enum facing{
        left =0,
        right =1,
    };
    facing facing = right;


    bool isUp_keyPressed = 0;
    bool isDown_keyPressed = 0;
    bool isRight_keyPressed = 0;
    bool isLeft_keyPressed = 0;

    //state of kirby

    enum move{
        move_stop,
        move_jump,
        move_flying
    };
    enum state{
        state_full, state_fire,
        state_spark, state_normal
    };

    state state = state_normal;
    move move = move_stop;

    const double jump_speed = -20;
    const double flying_speed = -10;
    double vy = jump_speed; // the start jumping speed 13
    const double gravity = 1;

    //previous x y
    int temp_x =0, temp_y =0;


};

#endif // KIRBY_H
