#include "kirby.h"
#include<iostream>
Kirby::Kirby(QString path, QGraphicsScene *Scene,double spw_x, double spw_y):GameObject(path, Scene)
{
    setZValue(1);
    setData(0,"Player");
    state =state_normal;
    move = move_ground;
    facing=right;
    scene = Scene;
    spaw_x=spw_x; spaw_y=spw_y;
}

void Kirby::updateMovement(int speed){
 /*   switch(move){
    case move_attacking:
        std::cout<<"attacking"<<std::endl;
        break;
    case move_flying:
        std::cout<<"flying"<<std::endl;
        break;
    case move_ground:
        std::cout<<"ground"<<std::endl;
        break;
    case move_hurt:
        std::cout<<"hurt"<<std::endl;
        break;
    case move_inhale:
        std::cout<<"inhale"<<std::endl;
        break;
    case move_jump:
        std::cout<<"jump"<<std::endl;
        break;
    case move_stop:
        std::cout<<"stop"<<std::endl;
        break;

    }*/

    temp_x = x();

    if(isYWeird()){
        setY(y_pre_frame);
        return;
    }
    y_pre_frame=y();

//attack & inhale
    if(isXPressed)
    {
        if(state!=state_normal)
        {
            if(state==state_full&&!isXreleaseAfterInhale)
            {
                //nothing
            }
            else if(move!= move_attacking)//isXreleasedAfterPress&&
            {
                move=move_attacking;
                Attk_frames=0;
                //isXreleasedAfterPress=0;
            }

        }
        else if(move==move_ground&&state!=state_full)//if state is normal
            move=move_inhale;

    }else
    {
        isXreleaseAfterInhale=true;
        if(move==move_inhale)
        {
            inhale_frames=0;
            move=move_ground;
            setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/kirby_stop_R.png":":/Image/Kirby_normal/kirby_stop_L.png"));
        }
       // isXreleasedAfterPress=1;
    }

    if(move==move_inhale){
        moveInhale();
        return;
    }
    if(move==move_attacking){
        moveAttack();
        return;
    }
    setOffset(0,0);


//enemy collision
    if(isEnemy()&&move!=move_attacking&&hurt_frames==120)
    {
        handelCollideEnemy();

    }
    //hurt animation
    if(hurt_frames<120)
    {
        moveHurt();
    }
    if(hurt_frames<15)
    {//knockback within the 0.25s after collide with enemy
     //here only calculate the x, the gravity is dealing the y
        setX(x()+vx);
        if(facing==left) vx+=friction;
        else vx-=friction;
    }else vx=0;

//x direction
    if(state==state_full)
        setOffset(0,-20);
    else setOffset(0,0);
    if(isDown_keyPressed&&!(move==move_flying)&&!(move==move_jump)) moveDown();
    else
    {
        if(isLeft_keyPressed && isRight_keyPressed) moveStop();
        else{
            if(isLeft_keyPressed){ moveLeft(speed); Right_frames = 0;}
            else if(isRight_keyPressed){ moveRight(speed); Left_frames = 0;}
        }
        handleCollisionX();
//y direction
        temp_y = y();
        if(Down_time >0){
             Down_time = 0;
            }
        if(isUp_keyPressed&&state!=state_full){
            moveUp();
            if(isGrounded&&!isAir){
                Up_frames=0;
                move=move_jump;
                vy = jump_speed;
                isGrounded=0;
                Uptime=1;
            }else if(Uptime==0){
                state=state_air;
                move=move_flying;
                vy = flying_speed;
                Uptime=1;
            }
        }else Uptime=0;

       if(move == move_flying||isAir){
           vy+=0.5;
           if(vy>2.5) vy=2.5;
       }else{
            //gravity
            vy += gravity;
            if(vy>(-jump_speed)) vy=-jump_speed;
        }

        //for animation in air
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
    if(move == move_ground && state==state_normal&&!isAir){
        setOffset(0,0);
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
    }else if(isAir && move==move_ground){

        setOffset(0,0);
            if(Left_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_1_L.png"));
                Left_frames++;
            }else if(Left_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_2_L.png"));
                Left_frames++;
            }else Left_frames = 0;
    }else if(state==state_full)
    {
        if(Left_frames < image_frame){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_full_run_L.png").scaledToHeight(173));
            Left_frames++;
        }else if(Left_frames < image_frame *2){
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_full_run(2)_L.png").scaledToHeight(173));
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
    if(state==state_normal&&move == move_ground&&!isAir){
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

    }else if(isAir&&move == move_ground){
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
    else if(state==state_full)
        {
            if(Right_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_full_run_R.png").scaledToHeight(173));
                Right_frames++;
            }else if(Right_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_full_run(2)_R.png").scaledToHeight(173));
                Right_frames++;
            }else Right_frames = 0;
        }
        setPos(x() + speed, y());

}
void Kirby::moveUp(){
    if(move == move_flying){//flying animation
        isAir=1;
    if(isAir){
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
        setOpacity(0.5f);
    } else {
        setOpacity(1.0f);
    }
    hurt_frames++;
}

void Kirby::moveInhale()
{
    move =move_inhale;
    isXreleaseAfterInhale=false;

    //pull enemy
    QRectF inhaleZone;
    if (facing == right)
    {
        inhaleZone = QRectF(x() + 122, y(), 300, pixmap().height());

    } else {
        inhaleZone = QRectF(x() - 300, y(), 300, pixmap().height());
    }

    QList<QGraphicsItem*> items_in_zone = scene->items(inhaleZone);
        for(QGraphicsItem* item : items_in_zone) {
            if(item->data(0) == "Enemy"&&item->data(1)=="halable") {
                double enemy_x = item->x();
                double enemy_y = item->y();

                Enemy* enemy = dynamic_cast<Enemy*>(item);
                double pull_speed = 5.0;

                if (enemy_x > x()) item->setX(enemy_x - pull_speed);
                else item->setX(enemy_x + pull_speed);

                if (enemy_y > y()) item->setY(enemy_y - pull_speed);
                else item->setY(enemy_y + pull_speed);

                double dis_x = std::abs(x()-enemy_x);
                double dis_y = std::abs(y()-enemy_y);
                if(dis_x<160&&dis_y<160){
                    enemy->setDead(true);
                    move=move_ground;
                    state=state_full;
                    //isXreleasedAfterPress=0;
                    setOffset(0,-50);
                    setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/kirby_full_stop_R.png":":/Image/Kirby_normal/kirby_full_stop_L.png").scaledToHeight(180));
                    return;
                }
            }
        }

    //animation
        const double offset_y=-50;
    if(facing==right)
    {

        inhale_frames++;
        if((inhale_frames/10)%2==0)
        {
            setPixmap(QPixmap(":/Image/Kirby_normal/inhale_R.png").scaledToHeight(202));
            setOffset(0,offset_y);

        }else{ setPixmap(QPixmap(":/Image/Kirby_normal/inhale(2)_R.png").scaledToHeight(202));
        setOffset(0,offset_y);
        }
    }else if(facing==left)
    {
        inhale_frames++;
        if((inhale_frames/10)%2==0)
        {
            setPixmap(QPixmap(":/Image/Kirby_normal/inhale_L.png").scaledToHeight(202));
             setOffset(-190,offset_y);
        }else {setPixmap(QPixmap(":/Image/Kirby_normal/inhale(2)_L.png").scaledToHeight(202));
            setOffset(-190,offset_y);}

    }


}



void Kirby::moveAttack(){
    move=move_attacking;
    Attk_frames++;
    switch(state){
    case state_air:
        isAir=0;
        Uptime=0;
        switch (facing){
        case left:
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_attack_L.png"));
                if(Attk_frames<image_frame){
                    Attk_frames++;
                }else if(Attk_frames<image_frame*2){
                    Attk_frames++;
                }else { Attk_frames=0;move=move_ground;state=state_normal;std::cout<<"here"<<std::endl;
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_L.png"));}
                    break;
        case right:
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_attack_R.png"));
                if(Attk_frames<image_frame){
                    Attk_frames++;
                }else if(Attk_frames<image_frame*2){
                    Attk_frames++;
                }else { Attk_frames=0;move=move_stop;state=state_normal;
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_R.png"));}
                    break;
            break;
        }
    break;
    case state_fire:
        switch (facing){
        case left:
                if(Attk_frames<image_frame){
                    Attk_frames++;
                    setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attk(2)_L.png"));
                }else if(Attk_frames<image_frame*2){
                    setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attack_L.png"));
                    Attk_frames++;
                }else if(Attk_frames<image_frame*3){
                    setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attk(3)_L.png"));
                    Attk_frames++;
                }else{ Attk_frames=0;}
                    break;
        case right:
                if(Attk_frames<image_frame){
                    Attk_frames++;
                    setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attk(2)_R.png"));
                }else if(Attk_frames<image_frame*2){
                    Attk_frames++;
                    setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attack_R.png"));
                }else if(Attk_frames<image_frame*3){
                    Attk_frames++;
                    setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attk(3)_R.png"));
                }else Attk_frames=0;
                    break;
            break;
        }
    break;
    case state_spark:
        if(Attk_frames<image_frame)
        {
            setPixmap(QPixmap(":/Image/Kirby_spark/kirby_spark_attack.png"));
            Attk_frames++;
        }else if(Attk_frames<image_frame*2){
            setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_attack(2).png"));
            Attk_frames++;
        }else if(Attk_frames<image_frame*3){
            setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_attack(3).png"));
            Attk_frames++;
        }else if(Attk_frames<image_frame*4){
            setPixmap(QPixmap(":/Image/Kirby_spark/kirby_spark_attack(4).png"));
            Attk_frames++;
        }else if(Attk_frames<image_frame*5){
            setPixmap(QPixmap(":/Image/Kirby_spark/kirby_spark_attack(5).png"));
            Attk_frames++;
        }else{ Attk_frames=0;}

    break;
    case state_full:
        //just for now
        move=move_ground;
        Attk_frames=0;
        state=state_normal;
    break;
    default:
    move=move_stop;
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

bool Kirby::isYWeird(){
    bool maybe=0;
    if(y()-y_pre_frame>30 || y()-y_pre_frame<-30)
    {maybe=1;}
    return maybe;
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


void Kirby::handelCollideEnemy(){
    move=move_ground;
    hp--;
    state=state_normal;
    isAir=0;
    hurt_frames=0;
    QList<QGraphicsItem*> enemys= scene->collidingItems(this);
    for(QGraphicsItem* item : enemys){
        if(item->data(0)=="Enemy"){
            if(item->x() > x())
            {
                vx=-knockback_sped;
            }else
                vx=knockback_sped;
            vy=knockup_sped;
            return ;
        }
    }
    return;
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

bool Kirby::isEnemy(Enemy** output_ememy){
    QList<QGraphicsItem*> items_hit = scene-> collidingItems(this);
    for(QGraphicsItem* item : items_hit)
        if(item->data(0)=="Enemy")
        {
            Enemy* eney = dynamic_cast<Enemy*>(item);
            if(output_ememy!=nullptr){
                *output_ememy=eney;
                if((*output_ememy)->getType()!=1)//type!=Gordo
                    (*output_ememy)->setDead(true);
            }
                return true;

        }
    return false;
}


