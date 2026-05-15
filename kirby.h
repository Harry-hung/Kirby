#ifndef KIRBY_H
#define KIRBY_H

#include "gameobject.h"
#include <QKeyEvent>
//Special class of gameObject: player

class Kirby : public GameObject
{
public:
    Kirby(QString img_path, QGraphicsScene *scene);

    void moveLeft(int speed);
    void moveRight(int speed);
    void moveUp(int speed);
    void moveDown();
    void moveStop();
    void updateMovement(int speed);

    void handlePressEvent(QKeyEvent *event);
    void handleReleaseEvent(QKeyEvent *event);
private:

    //
    QGraphicsScene *scene=nullptr;
    //frame per image
    int Left_time =0;//how many frames does Key_Left pressed
    int Right_time = 0;//how many frames does Key_Right pressed
    int Down_time = 0;
    const int image_frame = 15;//the duration of a image in frames(image per frame)

    //where kirby faceing left=0, right=1
    bool facing = right;
    const bool right =1;
    const bool left =0;

    bool isUp_keyPressed = 0;
    bool isDown_keyPressed = 0;
    bool isRight_keyPressed = 0;
    bool isLeft_keyPressed = 0;

    //state of kirby
    int state = state_normal;
    const int state_normal = 0;
    const int state_jump = 1;
    const int state_flying = 2;
    const int state_full = 3;
    const int state_fire = 4;
    const int state_spark = 5;

    //previous x y, for better animation because the down picture is weird
    int temp_x =0, temp_y =0;
};

#endif // KIRBY_H
