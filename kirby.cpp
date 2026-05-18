#include "kirby.h"
#include<iostream>
Kirby::Kirby(QString path, QGraphicsScene *Scene):GameObject(path, Scene)
{
    setZValue(1);
    state =state_normal;
    move = move_stop;
    facing=right;
    Kirby::scene = Scene;
}

void Kirby::updateMovement(int speed){
std::cout<<isDoor;
    temp_x = x();

    if(isDown_keyPressed) moveDown();
    else{
        if(isLeft_keyPressed && isRight_keyPressed&&isGrounded) moveStop();
        else{
            if(isLeft_keyPressed){ moveLeft(speed); Right_frames = 0;}
            else if(isRight_keyPressed){ moveRight(speed); Left_frames = 0;}
        }
        handleCollisionX();

        temp_y = y();
        if(Down_time >0){
             Down_time = 0;
            }
        if(isUp_keyPressed){
            if(isDoor()) {
                ////
            }
            moveUp();
            if(isGrounded){
                Up_frames=0;
                move=move_jump;
                vy = jump_speed;
                isGrounded=0;
                Uptime=1;

            }else if(Uptime==0){
                move=move_flying;
                vy = flying_speed;
                Uptime=1;
            }
        }else Uptime=0;
        if(move == move_flying){
            vy+=0.5;
            if(vy>2.5) vy=2.5;

       }else{
            vy += gravity;
            if(vy>(-jump_speed)) vy=-jump_speed;
        }

        //for animation
        if(move==move_flying||move==move_jump) moveUp();

        setY(y()+vy);

    }

        handleCollisionY();

        if(!(isRight_keyPressed| isLeft_keyPressed|isUp_keyPressed|isDown_keyPressed)&&isGrounded)
        {
            temp_x =x(); temp_y = y();
            for(int i=0;i<=10;i++){
                setY(temp_y-i);
                for(int j=0;j<=10;j++){
                    setX(temp_x-j);
                    if(!isCollision())
                        break;
                            }
                if(!isCollision())
                    break;
            }
            setPos(temp_x,temp_y);
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
    if(state_normal){
        if(move == move_stop){
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
    }setPos(x() + speed, y());

}
void Kirby::moveUp(){
    if(move == move_flying){//flying animation
        if(facing==right){
            if(Up_frames<image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_1_R.png"));
                Up_frames++;
            }else if(Up_frames<2*image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_2_R.png"));
                Up_frames++;
                if(Up_frames==2*image_frame)
                    Up_frames=0;

            }
        }else if(facing==left){
            if(Up_frames<image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_1_L.png"));
                Up_frames++;
            }else if(Up_frames<2*image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_2_L.png"));
                Up_frames++;
                if(Up_frames==2*image_frame)Up_frames=0;
                }
            }
    }else if(move==move_jump){//jump motion
        if(facing==right){
            if(vy<-3){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_jump(1).png"));
            }else setPixmap(QPixmap(":/Image/Kirby_normal/kirby_jump(3).png"));

        }else if(facing==left){
            if(vy<(-3)){
                setPixmap(QPixmap::fromImage(QImage(":/Image/Kirby_normal/kirby_jump(1).png").mirrored(1,0)));
            }else {
                setPixmap(QPixmap::fromImage(QImage(":/Image/Kirby_normal/kirby_jump(3).png").mirrored(1,0)));
            }
        }
    }

}

void Kirby::moveDown(){
        if(facing == left){
            if(Down_time ==0){
                setScale(0.9);
                Down_time++;
            }
            if(state==state_normal)
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_down_L.png"));
            else if(state==state_fire)
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_down_L.png"));
        }if(facing == right){
            if(Down_time ==0){
                setScale(0.9);
                Down_time++;
            }
            if(state==state_normal)
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_down_R.png"));
            else if(state==state_fire)
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_down_R.png"));
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


//we first deal collision on x then deal y, because we need to see if Kirby is in air
//Dont Change the order!!!!
void Kirby::handleCollisionX(){
    int maxWalkingHeight = 10;//the highest pixel kirby can walk off
    if(isCollision()){
        int temptemp_y = y();
        for(int i=1;i<=maxWalkingHeight;i++){
           setY(y()-i);
           if(!isCollision())
            return;
        }

        if(isCollision()){
            setX(temp_x);
            setY(temptemp_y);
            move=move_stop;
        }

    }

}

void Kirby::handleCollisionY(){
    if(!isCollision()){
        isGrounded = 0;
        return;
    }

    if(y() > temp_y){
        while(isCollision()){
            setY(y()-1);
        }
        vy=0;
        isGrounded=1;
        move=move_stop;
    }
}


bool Kirby::isCollision(){
    QList<QGraphicsItem*> items_hit = scene-> collidingItems(this);
    for(QGraphicsItem* item : items_hit){
        if(item->data(0)=="Solid"){
            return true;
        }
        if(item->data(0)=="Platform"){
            if(item->y() > y()+pixmap().height()-50)
                return true;
        }
    }
    return false;
}

bool Kirby::isDoor(){
    QList<QGraphicsItem*> items_hit = scene-> collidingItems(this);
    for(QGraphicsItem* item : items_hit)
        if(item->data(0)=="Door")
            return true;

}

bool Kirby::isEnemy(){
    QList<QGraphicsItem*> items_hit = scene-> collidingItems(this);
    for(QGraphicsItem* item : items_hit)
        if(item->data(0)=="Enemy")
            return true;

}


