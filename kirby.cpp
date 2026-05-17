#include "kirby.h"

Kirby::Kirby(QString path, QGraphicsScene *Scene):GameObject(path, Scene)
{
    setZValue(1);
    state =state_normal;
    move = move_stop;
    facing=right;
    Kirby::scene = Scene;
}

void Kirby::updateMovement(int speed){
    temp_x = x();temp_y = y();
    if(isDown_keyPressed) moveDown();
    else{
        if(isLeft_keyPressed && isRight_keyPressed) moveStop();
        else{
            if(isLeft_keyPressed){ moveLeft(speed); Right_frames = 0;}
            else if(isRight_keyPressed){ moveRight(speed); Left_frames = 0;}
        }
        handleCollision();

        if(Down_time >0){
             Down_time = 0;
            }
        if(isUp_keyPressed){
            moveUp();
            Uptime=1;
        }else if(!isGrounded){
            if(move==move_flying||move==move_jump) setPos(x(),y()+5);
            else setPos(x(),y()+10);
        }
        handleCollision();

        if(!(isRight_keyPressed| isLeft_keyPressed|isUp_keyPressed|isDown_keyPressed))
            moveStop();

    }


}

void Kirby::moveLeft(int speed){

    facing = left;
    if(x()-speed<0){
        moveStop();
        return;
    }

    setPos(x() - speed, y());
    if(state == state_normal && move == move_stop){
        if(Left_frames <image_frame){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_1_L.png"));
            Left_frames++;
        } else if(Left_frames <2*image_frame){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_2_L.png"));
            Left_frames++;
        } else if(Left_frames < 3*image_frame){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_3_L.png"));
            Left_frames++;
        } else Left_frames = 0;
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
        if(Right_frames < image_frame){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_1_R.png"));
            Right_frames++;
        }else if(Right_frames < image_frame *2){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_2_R.png"));
            Right_frames++;
        }else if(Right_frames < image_frame *3){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_run_3_R.png"));
            Right_frames++;
        }else Right_frames = 0;
     }
}
void Kirby::moveUp(){
    setPos(x(), y()-6);
    if(Uptime>0)
        move = move_flying;
    else move = move_jump;

    //up_key release
    if(!isUp_keyPressed)
        Uptime=0;
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

void Kirby::Animation(){

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

void Kirby::handleCollision(){
    int maxWalkingHeight = 5;//the highest pixel kirby can walk off
    if(isCollision()){
        for(int i=1;i<=maxWalkingHeight;i++){
           setY(y()-i);
           if(!isCollision()) { isGrounded = 0;move=move_stop;return;}
        }
        if(isCollision()){
            setY(temp_y);
            isGrounded = 0;
            move=move_stop;
        }
        if(isCollision()){
            setX(temp_x);
            isGrounded = 0;
        }

    }

}

bool Kirby::isCollision(){
    QList<QGraphicsItem*> items_hit = scene-> collidingItems(this);
    for(QGraphicsItem* item : items_hit){
        if(item->data(0)=="Solid"){
            return true;
        }
    }
    return false;
}
