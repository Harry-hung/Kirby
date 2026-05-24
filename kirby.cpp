#include "kirby.h"
#include<iostream>
Kirby::Kirby(QString path, QGraphicsScene *Scene,double spw_x, double spw_y):GameObject(path, Scene)
{
    setZValue(1);
    state =state_normal;
    move = move_ground;
    facing=right;
    scene = Scene;
    spaw_x=spw_x; spaw_y=spw_y;
}

void Kirby::updateMovement(int speed){
    temp_x = x();

    if(isEnemy()&&move!=move_attacking&&hurt_frames==120)
    {
        CollideEnemy();
        move=move_ground;
        hp--;
        state=state_normal;
        hurt_frames=0;
    }
    if(isYWeird()){
        setY(y_pre_frame);
        return;
    }
    y_pre_frame=y();
    if(move==move_attacking)
    {
        moveAttack();
        return;
    }
    if(hurt_frames<120)
        moveHurt();

    if(isDown_keyPressed&&!(move==move_flying)&&!(move==move_jump)) moveDown();
    else
    {
        if(isLeft_keyPressed && isRight_keyPressed) moveStop();
        else{
            if(isLeft_keyPressed){ moveLeft(speed); Right_frames = 0;}
            else if(isRight_keyPressed){ moveRight(speed); Left_frames = 0;}
        }
        handleCollisionX();

        temp_y = y();
        if(Down_time >0){
             Down_time = 0;
            }
        if(isUp_keyPressed&&move){
            moveUp();
            if(isGrounded&&state!=state_air){
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
        if(move == move_flying||state==state_air){
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
        for(int i=0;i<=10;i++)
        {
            setY(temp_y-i);
            for(int j=0;j<=10;j++)
            {
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
    if(move == move_ground && state==state_normal){
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
    }else if(state==state_air && move==move_ground){
            if(Left_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_1_L.png"));
                Left_frames++;
            }else if(Left_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_2_L.png"));
                Left_frames++;
            }else Left_frames = 0;
        }
}

void Kirby::moveRight(int speed){
    facing = right;
    if(x()+speed > (scene->sceneRect().width()-QPixmap(":/Image/Kirby_normal/kirby_stop_R.png").width())){
        moveStop();
        return;
    }
    if(state==state_normal&&move == move_ground){
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

    }else if(state==state_air&&move == move_ground){
        if(move==move_ground){
            if(Right_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_1_R.png"));
                Right_frames++;
            }else if(Right_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_2_R.png"));
                Right_frames++;
            }else Right_frames = 0;
         }
        }

        setPos(x() + speed, y());

}
void Kirby::moveUp(){
    if(move == move_flying){//flying animation
        state=state_air;
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

void Kirby::moveHurt()
{
    if ((hurt_frames / 4) % 2 == 0) {
        setOpacity(0.3f);
    } else {
        setOpacity(1.0f);
    }
    hurt_frames++;
}

bool Kirby::isYWeird(){
    bool maybe=0;
    if(y()-y_pre_frame>30 || y()-y_pre_frame<-30)
    {maybe=1;}
    return maybe;
}

void Kirby::moveAttack(){
    if(state==state_air){
        switch (facing){
        case left:
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_attack_L.png"));
            if(Attk_frames<image_frame){
                Attk_frames++;
            }else if(Attk_frames<image_frame*2){
                Attk_frames++;
            }else Attk_frames=0;
                break;
        case right:
            if(Attk_frames<image_frame) ;
        break;
        }
    }
}

void Kirby::handlePressEvent(QKeyEvent *event){
    if (event->isAutoRepeat()) {
            return;
        }
    if(event->key() == Qt::Key_X) isXPressed =1;
    if(event->key() == Qt::Key_Up) isUp_keyPressed =1;
    if(event->key() == Qt::Key_Down) isDown_keyPressed =1;
    if(event->key() == Qt::Key_Right) isRight_keyPressed =1;
    if(event->key() == Qt::Key_Left) isLeft_keyPressed =1;

}

void Kirby::handleReleaseEvent(QKeyEvent *event){
    if (event->isAutoRepeat()) {
        return;
    }
    if(event->key() == Qt::Key_X) isXPressed =0;
    if(event->key() == Qt::Key_Up) isUp_keyPressed =0;
    if(event->key() == Qt::Key_Down) isDown_keyPressed =0;
    if(event->key() == Qt::Key_Right) isRight_keyPressed =0;
    if(event->key() == Qt::Key_Left) isLeft_keyPressed =0;

}


//we first deal collision on x then deal y, because we need to see if Kirby is in air
//Dont Change the order!!!!
void Kirby::handleCollisionX(){

    if(isCollision()){
        int temptemp_y = y();
        for(int i=1;i<=maxWalkingHeight;i++){
           setY(y()-i);
           if(!isCollision()){
                isWalkable=1;
               return;}
        }
        isWalkable=0;

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
    if(isYWeird()) setY(temp_y);
        vy=0;
        isGrounded=1;
        move=move_ground;
    }
}


bool Kirby::isCollision(){
    QList<QGraphicsItem*> items_hit = scene-> collidingItems(this);
    for(QGraphicsItem* item : items_hit){
        if(item->data(0)=="Solid"){
            return true;
        }

        //Platform can jump through if player is under it
        if(item->data(0)=="Platform"&&!isDown_keyPressed){
            if(item->y() > y()+pixmap().height()-35)
                return true;
        }
    }
    return false;
}


bool Kirby::CollideEnemy(){
    QList<QGraphicsItem*> enemys= scene->collidingItems(this);
    for(QGraphicsItem* item : enemys){
        if(item->data(0)=="Enemy"){
            if(item->x() > x())
            {
                setX(x()-20);
            }else
                setX(x()+20);
            return true;
        }
    }
    return false;
}

bool Kirby::isDoor(){
    QList<QGraphicsItem*> items_hit = scene-> collidingItems(this);
    for(QGraphicsItem* item : items_hit)
        if(item->data(0)=="Door")
            return 1;
    return 0;

}

bool Kirby::isNextScene(){
    if(isUp_keyPressed&&isDoor()){
        return 1;
    }
    return 0;
}

bool Kirby::isEnemy(){
    QList<QGraphicsItem*> items_hit = scene-> collidingItems(this);
    for(QGraphicsItem* item : items_hit)
        if(item->data(0)=="Enemy")
            return true;
    return 0;
}


