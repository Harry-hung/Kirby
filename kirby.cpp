#include "kirby.h"

Kirby::Kirby(QString path, QGraphicsScene *Scene):GameObject(path, Scene)
{
    setZValue(1);
    state =state_normal;
    facing=right;
    Kirby::scene = Scene;
}

void Kirby::updateMovement(int speed){
    if(isDown_keyPressed) moveDown();
    else{
        if(Down_time >0){
             Down_time = 0;
            }
        if(isUp_keyPressed) moveUp(speed);
        if(isLeft_keyPressed && isRight_keyPressed) moveStop();
        else{
            if(isLeft_keyPressed){ moveLeft(speed); Right_time = 0;}
            else if(isRight_keyPressed){ moveRight(speed); Left_time = 0;}
        }
    }
    if(!(isRight_keyPressed| isLeft_keyPressed|isUp_keyPressed|isDown_keyPressed))
        moveStop();

}

void Kirby::moveLeft(int speed){

    facing = left;
    if(x()-speed<0){
        moveStop();
        return;
    }

    setPos(x() - speed, y());
    if(state == state_normal){
        if(Left_time <image_frame){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_1_L.png"));
            Left_time++;
        } else if(Left_time <2*image_frame){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_2_L.png"));
            Left_time++;
        } else if(Left_time < 3*image_frame){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_3_L.png"));
            Left_time++;
        } else Left_time = 0;
    }
}

void Kirby::moveRight(int speed){
    facing = right;
    if(x()+speed > (scene->sceneRect().width()-QPixmap(":/Image/Kirby_normal/kirby_stop_R.png").width())){
        moveStop();
        return;
    }
    setPos(x() + speed, y());
    if(state == state_normal){
        if(Right_time < image_frame){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_1_R.png"));
            Right_time++;
        }else if(Right_time < image_frame *2){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_2_R.png"));
            Right_time++;
        }else if(Right_time < image_frame *3){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_3_R.png"));
            Right_time++;
        }else Right_time = 0;
     }
}
void Kirby::moveUp(int speed){
    setPos(x(), y()-speed);
}

void Kirby::moveDown(){
    if(state == state_normal){
        if(facing == left){
            if(Down_time ==0){
                setScale(0.9);
                Down_time++;
            }
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_down_L.png"));
        }if(facing == right){
            if(Down_time ==0){
                setScale(0.9);
                Down_time++;
            }
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_down_R.png"));
        }
    }
}
void Kirby::moveStop(){

    setScale(1);
    if(state == state_normal){
        if(facing == left)
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_L.png"));
        else if(facing == right)
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_R.png"));
    }
}

void Kirby::handlePressEvent(QKeyEvent *event){
    if (event->isAutoRepeat()) {
            return;
        }
    if(event->key() == Qt::Key_Up) isUp_keyPressed =1;
    if(event->key() == Qt::Key_Down) isDown_keyPressed =1;
    if(event->key() == Qt::Key_Right) isRight_keyPressed =1;
    if(event->key() == Qt::Key_Left) isLeft_keyPressed =1;

}

void Kirby::handleReleaseEvent(QKeyEvent *event){
    if (event->isAutoRepeat()) {
        return;
    }

    if(event->key() == Qt::Key_Up) isUp_keyPressed =0;
    if(event->key() == Qt::Key_Down) isDown_keyPressed =0;
    if(event->key() == Qt::Key_Right) isRight_keyPressed =0;
    if(event->key() == Qt::Key_Left) isLeft_keyPressed =0;

}


