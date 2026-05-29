#include "kirby.h"
#include<iostream>
Kirby::Kirby(QString path, QGraphicsScene *Scene,double spw_x, double spw_y):GameObject(path, Scene)
{
    setZValue(3);
    setData(0,"Player");
    state =state_fire;
    move = move_ground;
    facing=right;
    scene = Scene;
    spaw_x=spw_x; spaw_y=spw_y;
    hp=3; life=3;
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



    //abandon ability
    if(isVPressed){
        if(isVrelease&&(state==state_fire||state==state_spark))
        {
            isVrelease=0;
            state=state_normal;
        }
    }else isVrelease =1;

    temp_x = x();

    //fall in hole
    if(y()>1680){
        hp=0;
        moveRespawn();
    }

    if(isYWeird()){
        setY(y_pre_frame);
        return;
    }



    y_pre_frame=y();



//attack & inhale
    if(isXPressed)
    {
        if(isXrelease&&hurt_frames>30){
            if(state!=state_normal){
                move=move_attacking;
            }else if(isGrounded){
                move=move_inhale;
            }
        }
        isXrelease=0;
    }else
    {
        isXrelease=true;
        if(move==move_inhale)
        {
            inhale_frames=0;
            setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/kirby_stop_R.png":":/Image/Kirby_normal/kirby_stop_L.png"));
            move=move_ground;
        }
        if(move==move_attacking&&state!=state_air&&state!=state_full&&!isAir)
        {
            setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/kirby_stop_R.png":":/Image/Kirby_normal/kirby_stop_L.png"));
            Attk_frames=0;
            moveStop();
            move=move_ground;
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
//get item
    get_item();

//enemy collision
    if(move!=move_attacking&&hurt_frames==120&&isEnemy())
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
            if(isGrounded&&!isAir&&state!=state_fire&&state!=state_spark){
                Up_frames=0;
                move=move_jump;
                vy = jump_speed;
                isGrounded=0;
                Uptime=1;
            }else if(Uptime==0||state==state_fire||state==state_spark){
                isAir=1;
                if(state!=state_spark&&state!=state_fire)
                    state=state_air;
                move=move_flying;

                if(y()>-70){
                    Uptime=1;

                    if(state==state_fire||state==state_spark)
                        vy = flying_speed*0.85;
                    else vy = flying_speed;
                }
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



    if(!(isRight_keyPressed||isLeft_keyPressed||isUp_keyPressed||isDown_keyPressed)&&move==move_ground)
    {
        if(move==move_jump)
            moveUp();
        else moveStop();
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
    }else if(isAir){
        setOffset(0,0);
        if(state==state_air){
            if(Left_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_1_L.png"));
                Left_frames++;
            }else if(Left_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_2_L.png"));
                Left_frames++;
            }else Left_frames = 0;
        }else if(state==state_fire)
        {
            if(Left_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_fly(1)_L.png"));
                Left_frames++;
            }else if(Left_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_fly(2)_L.png"));
                Left_frames++;
            }else Left_frames = 0;
        }else if(state==state_spark)
        {
            if(Left_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_fly(1)_L.png"));
                Left_frames++;
            }else if(Left_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_fly(2)_L.png"));
                Left_frames++;
            }else Left_frames = 0;
        }
    }else if(state==state_full)
    {
        if(Left_frames < image_frame){
            setOffset(0,-30);
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_full_run_L.png").scaledToHeight(173));
            Left_frames++;
        }else if(Left_frames < image_frame *2){
            setOffset(0,-30);
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_full_run(2)_L.png").scaledToHeight(173));
            Left_frames++;
        }else Left_frames = 0;
    }else if(state==state_fire)
    {
        if(Left_frames < image_frame){
            setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_run(1)_L.png").scaledToHeight(173));
            Left_frames++;
        }else if(Left_frames < image_frame *2){
            setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_run(2)_L.png").scaledToHeight(173));
            Left_frames++;
        }else if(Left_frames < image_frame *3){
            setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_run(3)_L.png").scaledToHeight(173));
            Left_frames++;
        }else Left_frames = 0;
    }else if(state==state_spark)
    {
        if(Left_frames < image_frame){
            setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_run(1)_L.png").scaledToHeight(173));
            Left_frames++;
        }else if(Left_frames < image_frame *2){
            setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_run(2)_L.png").scaledToHeight(173));
            Left_frames++;
        }else Left_frames = 0;
    }
}

void Kirby::moveRight(int speed){
    facing = right;
    if(x()+speed > (scene->sceneRect().width()-pixmap().width())){
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

    }else if(isAir){
        if(state==state_air){
            if(Right_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_1_R.png"));
                Right_frames++;
            }else if(Right_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_2_R.png"));
                Right_frames++;
            }else Right_frames = 0;
        }else if(state==state_fire){
            if(Right_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_fly(1)_R.png"));
                Right_frames++;
            }else if(Right_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_fly(2)_R.png"));
                Right_frames++;
            }else Right_frames = 0;
        }else if(state==state_spark){
            if(Right_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_fly(1)_R.png"));
                Right_frames++;
            }else if(Right_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_fly(2)_R.png"));
                Right_frames++;
            }else Right_frames = 0;
        }
        }
    else if(state==state_full)
        {
            if(Right_frames < image_frame){
                setOffset(0,-30);
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_full_run_R.png").scaledToHeight(173));
                Right_frames++;
            }else if(Right_frames < image_frame *2){
                setOffset(0,-30);
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_full_run(2)_R.png").scaledToHeight(173));
                Right_frames++;
            }else Right_frames = 0;
        }
    else if(state==state_fire)
        {

            if(Right_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_run(1)_R.png").scaledToHeight(173));
                Right_frames++;
            }else if(Right_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_run(2)_R.png").scaledToHeight(173));
                Right_frames++;
            }else if(Right_frames < image_frame *3){
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_run(3)_R.png").scaledToHeight(173));
                Right_frames++;
            }else Right_frames = 0;
        }
    else if(state==state_spark)
        {
            if(Right_frames < image_frame){
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_run(1)_R.png").scaledToHeight(173));
                Right_frames++;
            }else if(Right_frames < image_frame *2){
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_run(2)_R.png").scaledToHeight(173));
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
                if(state==state_air)
                    setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_1_R.png"));
                else if(state==state_fire){

                    setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_fly(1)_R.png"));
                }
                else if(state_spark){

                    setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_fly(1)_R.png"));
                }
                Up_frames++;
            }else if(Up_frames<2*image_frame){
                if(state==state_air)
                    setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_2_R.png"));
                else if(state==state_fire){

                    setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_fly(2)_R.png"));
                }else if(state_spark){

                    setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_fly(2)_R.png"));
                }
                Up_frames++;
                if(Up_frames==2*image_frame)
                    Up_frames=0;
            }
        }else if(facing==left){
            if(Up_frames<image_frame){
                if(state==state_air)
                    setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_1_L.png"));
                else if(state==state_fire){

                    setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_fly(1)_L.png"));
                }else if(state_spark){

                    setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_fly(1)_L.png"));
                }
                Up_frames++;
            }else if(Up_frames<2*image_frame){
                if(state==state_air)
                    setPixmap(QPixmap(":/Image/Kirby_normal/kirby_fly_2_L.png"));
                else if(state==state_fire){

                    setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_fly(2)_L.png"));
                }else if(state_spark){

                    setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_fly(2)_L.png"));
                }
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
setOffset(0,0);
        if(state==state_full){
            setOffset(0,0);
            setY(y()-20);
            if(enemy_type==Sparky) state=state_spark;
            else if(enemy_type==Hot_Head) state=state_fire;
            else state=state_normal;
        }else
        if(facing == left){
            if(Down_time ==0){
                setScale(0.9);
                Down_time++;
            }
            if(state==state_normal)
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_down_L.png"));
            else if(state==state_fire)
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_down_L.png"));
            else if(state==state_spark)
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_down_L.png"));
        }if(facing == right){
            if(Down_time ==0){
                setScale(0.9);
                Down_time++;
            }
            if(state==state_normal)
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_down_R.png"));
            else if(state==state_fire)
                setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_down_R.png"));
            else if(state==state_spark)
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_down_R.png"));

        }

}

void Kirby::moveStop(){
    setScale(1);
    if(state == state_normal&&!isAir){
        setOffset(0,0);
        if(facing == left)
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_L.png"));
        else if(facing == right)
            setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_R.png"));
    }else if(state==state_full){
        setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/kirby_full_stop_R.png":":/Image/Kirby_normal/kirby_full_stop_L.png").scaledToHeight(173));
    }else if(state==state_fire&&!isAir){
        setPixmap(QPixmap((facing==right)?":/Image/Kirby_fire/kirbyfire_stop_R.png":":/Image/Kirby_fire/kirbyfire_stop_L.png").scaledToHeight(173));
    }else if(state==state_spark&&!isAir){
        setPixmap(QPixmap((facing==right)?":/Image/Kirby_spark/Kirby_spark_stop_R.png":":/Image/Kirby_spark/Kirby_spark_stop_L.png").scaledToHeight(173));
    }
}

void Kirby::moveHurt()
{
    if(hp<=0){
        moveRespawn();
    }
    if ((hurt_frames / 4) % 2 == 0) {
        setOpacity(0.5f);
    } else {
        setOpacity(1.0f);
    }
    hurt_frames++;
}

void Kirby::moveRespawn(){
    vx=0;
    setPos(spaw_x,spaw_y);
    life--;
     y_pre_frame=y();
    if(life>=0)
        hp=3;
}

void Kirby::moveInhale()
{
    move =move_inhale;

    //pull enemy
    QRectF inhaleZone;
    if (facing == right)
    {
        inhaleZone = QRectF(x() + 122, y()-50, 300, pixmap().height());

    } else {
        inhaleZone = QRectF(x() - 300, y()-50, 300, pixmap().height());
    }
/*              if (!debugFireRect) {
                        // 如果方框還沒被建立，就建立一個紅色的空心方框 (線條粗細為 2)
                        QPen redPen(Qt::red, 2);
                        debugFireRect = scene->addRect(inhaleZone, redPen);
                        debugFireRect->setZValue(10); // 確保這個除錯框會蓋在角色和地圖最上層
                    } else {
                        // 如果方框已經存在，就更新它的位置與大小，並確保它是顯示狀態
                        debugFireRect->setRect(inhaleZone);
                        debugFireRect->setVisible(true);}
*/

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
                //Eat
                if(dis_x<160&&dis_y<160){
                    enemy->instantDead();
                    move=move_ground;
                    state=state_full;

                    enemy_type=enemy->getType();
                    setOffset(0,-50);
                    setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/kirby_full_stop_R.png":":/Image/Kirby_normal/kirby_full_stop_L.png").scaledToHeight(180));
                    return;
                    //isXreleasedAfterPress=0;
                }
            }
        }

    //enemy hit
    QRectF hitbox;
    if (facing == right)
    {
        hitbox = QRectF(x()+60, y()+20, 70, 70);
    } else {
        hitbox= QRectF(x()+60, y()+20, 70, 70);
    }
 /*   if (!debugFireRect) {
                            // 如果方框還沒被建立，就建立一個紅色的空心方框 (線條粗細為 2)
                            QPen redPen(Qt::red, 2);
                            debugFireRect = scene->addRect(hitbox, redPen);
                            debugFireRect->setZValue(10); // 確保這個除錯框會蓋在角色和地圖最上層
                        } else {
                            // 如果方框已經存在，就更新它的位置與大小，並確保它是顯示狀態
                            debugFireRect->setRect(hitbox);
                            debugFireRect->setVisible(true);}
    */
    handelEnemyinHitbox(hitbox);

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

    switch(state){
    case state_air:
        isAir=0;
        setOffset(0,0);
        Uptime=0;
        switch (facing){
        case left:
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_attack_L.png"));
                if(Attk_frames<2){
                    scene->addItem(new Projectile(scene,x()-50,y()+10,"Enemy",facing,"Gas"));
                    Attk_frames++;
                }else if(Attk_frames<15){
                    Attk_frames++;
                }else { Attk_frames=0;move=move_ground;state=state_normal;
                    setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_L.png"));
                }
                    break;
        case right:
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_attack_R.png"));
                if(Attk_frames<2){
                    scene->addItem(new Projectile(scene,x()+50,y()+10,"Enemy",facing,"Gas"));
                    Attk_frames++;
                }else if(Attk_frames<15){
                    Attk_frames++;
                }else { Attk_frames=0;move=move_ground;state=state_normal;
                    setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_R.png"));
                }
        break;

        }
    break;
    case state_fire:

        if(isAir)
        {
            Uptime=0;

            switch (facing){
            case left:
                    setPixmap(QPixmap(":/Image/Kirby_fire/fire_ha_L.png").scaledToHeight(220));
                    if(Attk_frames<2){
                        scene->addItem(new Projectile(scene,x()-50,y()+50,"Enemy",facing,"Gas"));
                        Attk_frames++;
                    }else if(Attk_frames<15){
                        Attk_frames++;
                    }else { Attk_frames=0;move=move_ground;isAir=0;//dont touch isAir
                        setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_stop_L.png").scaledToHeight(173));
                    }
                        break;
            case right:
                    setPixmap(QPixmap(":/Image/Kirby_fire/fire_ha_R.png").scaledToHeight(220));
                    if(Attk_frames<2){
                        scene->addItem(new Projectile(scene,x()+50,y()+50,"Enemy",facing,"Gas"));
                        Attk_frames++;
                    }else if(Attk_frames<15){
                        Attk_frames++;
                    }else { Attk_frames=0;move=move_ground;isAir=0;
                        setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_stop_R.png").scaledToHeight(173));
                    }
                        break;
                break;
            }
        }

        else if(isGrounded)
        {
            fire_attk();
            double fire_height=250;
            int frame_interval = 8;
            QRectF kirbyHitbox;
            switch (facing){
            case left:
                    setOffset(-250,-50);
                    if((Attk_frames/frame_interval)%3==0){
                        Attk_frames++;
                        setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attk(2)_L.png").scaledToHeight(fire_height));
                    }else if((Attk_frames/frame_interval)%3==1){
                        setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attack_L.png").scaledToHeight(fire_height));
                        Attk_frames++;
                    }else if((Attk_frames/frame_interval)%3==2){
                        setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attk(3)_L.png").scaledToHeight(fire_height));
                        Attk_frames=0;
                    }
                    kirbyHitbox=QRectF(x()+100,y()+50,75,75);
                        break;
            case right:
                kirbyHitbox=QRectF(x()+90,y()+50,75,75);
                    setOffset(0,-50);
                    if((Attk_frames/frame_interval)%3==0){
                        setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attk(2)_R.png").scaledToHeight(fire_height));
                        Attk_frames++;
                    }else if((Attk_frames/frame_interval)%3==1){
                        Attk_frames++;
                        setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attack_R.png").scaledToHeight(fire_height));
                    }else if((Attk_frames/frame_interval)%3==2){
                        setPixmap(QPixmap(":/Image/Kirby_fire/kirby_fire_attk(3)_R.png").scaledToHeight(fire_height));
                        Attk_frames=0;
                    }
                        break;

            }
            handelEnemyinHitbox(kirbyHitbox);

/*            if (!debugFireRect) {
                    // 如果方框還沒被建立，就建立一個紅色的空心方框 (線條粗細為 2)
                    QPen redPen(Qt::red, 2);
                    debugFireRect = scene->addRect(kirbyHitbox, redPen);
                    debugFireRect->setZValue(10); // 確保這個除錯框會蓋在角色和地圖最上層
                } else {
                    // 如果方框已經存在，就更新它的位置與大小，並確保它是顯示狀態
                    debugFireRect->setRect(kirbyHitbox);
                    debugFireRect->setVisible(true);}
*/
        }
    break;
    case state_spark:
        if(isAir)
        {
            Uptime=0;
            setOffset(0,0);
            switch (facing){
            case left:
                    setPixmap(QPixmap(":/Image/Kirby_spark/kirby_sparky_ha_L.png").scaledToHeight(220));
                    if(Attk_frames<2){
                        scene->addItem(new Projectile(scene,x()-50,y()+70,"Enemy",facing,"Gas"));
                        Attk_frames++;
                    }else if(Attk_frames<15){
                        Attk_frames++;
                    }else { Attk_frames=0;move=move_ground; isAir=0;
                        setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_stop_L.png").scaledToHeight(173));
                    }
                        break;
            case right:
                    setPixmap(QPixmap(":/Image/Kirby_spark/kirby_sparky_ha_R.png").scaledToHeight(220));
                    if(Attk_frames<2){
                        scene->addItem(new Projectile(scene,x()+100,y()+70,"Enemy",facing,"Gas"));
                        Attk_frames++;
                    }else if(Attk_frames<15){
                        Attk_frames++;
                    }else { Attk_frames=0;move=move_ground;isAir=0;
                        setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_stop_R.png").scaledToHeight(173));
                    }

                break;
            }
        }else if(isGrounded)
        {
            QRectF kirbyHitbox;
            spark_attk();
            if(Attk_frames<2)
            {
                setOffset(0,0);
                setPixmap(QPixmap(":/Image/Kirby_spark/kirby_spark_attack.png").scaledToHeight(340));
                Attk_frames++;
           }else if((Attk_frames%10)/4==0){
                setOffset(-70,-70);
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_attack(2).png").scaledToHeight(340));
                Attk_frames++;
            }else if((Attk_frames%10)/4==1){
                setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_attack(3).png").scaledToHeight(340));
                Attk_frames++;
            }else if((Attk_frames%10)/4==2){
                setPixmap(QPixmap(":/Image/Kirby_spark/kirby_spark_attack(4).png").scaledToHeight(340));
                Attk_frames++;
            }else if((Attk_frames%10)/4==3){
                setPixmap(QPixmap(":/Image/Kirby_spark/kirby_spark_attack(5).png").scaledToHeight(340));
                Attk_frames++;
            }else{ Attk_frames=0;}
            kirbyHitbox = QRectF(x()+100,y()+100,50,50);
            handelEnemyinHitbox(kirbyHitbox);

            /*if (!debugFireRect) {
                    // 如果方框還沒被建立，就建立一個紅色的空心方框 (線條粗細為 2)
                    QPen redPen(Qt::red, 2);
                    debugFireRect = scene->addRect(kirbyHitbox, redPen);
                    debugFireRect->setZValue(10); // 確保這個除錯框會蓋在角色和地圖最上層
                } else {
                    // 如果方框已經存在，就更新它的位置與大小，並確保它是顯示狀態
                    debugFireRect->setRect(kirbyHitbox);
                    debugFireRect->setVisible(true);}
*/
}

    break;
    case state_full:

    switch(facing){
        case left:
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_attack_L.png"));
                if(Attk_frames==0){
                   // std::cout<<"一千億個星星";
                    scene->addItem(new Projectile(scene,x()-50,y()+10,"Enemy",facing,"Star"));
                    Attk_frames++;
                }else if(Attk_frames<image_frame*1.2){
                    Attk_frames++;
                }else { Attk_frames=0;move=move_ground;state=state_normal;//std::cout<<"here"<<std::endl;
                    setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_L.png"));
                }
        break;
        case right:
                setPixmap(QPixmap(":/Image/Kirby_normal/kirby_attack_R.png"));
                if(Attk_frames==0){
                   // std::cout<<"一千億個星星";
                    scene->addItem(new Projectile(scene,x()+50,y()+10,"Enemy",facing,"Star"));
                    Attk_frames++;
                }else if(Attk_frames<image_frame*1.2){
                    Attk_frames++;
                }else { Attk_frames=0;move=move_ground;state=state_normal;
                    setPixmap(QPixmap(":/Image/Kirby_normal/kirby_stop_R.png"));
                }
        break;

    }
    break;
    default:
    move=move_ground;
    break;
    }
}

void Kirby::spark_attk()
{
    if(state==state_spark&&move==move_attacking){
        QList<QGraphicsItem*> items=scene->collidingItems(this);
        for(QGraphicsItem* item:items)
        {
            if(item->data(0)=="Enemy")
            {
                Enemy* ene = dynamic_cast<Enemy*>(item);
                if(ene->data(1)!="Inhalable")
                    ene->setDead(true);
            }
        }
    }
}

void Kirby::fire_attk()
{
    double rec_width=250;
    double rec_height=130;
    QRectF fireZone;
    if(facing==right){
        fireZone=QRectF(x()+150,y()+27,rec_width,rec_height);
    }else
        fireZone=QRectF(x()-200,y()+30,rec_width,rec_height);

    if(state==state_fire&&move==move_attacking){
        QList<QGraphicsItem*> itemsinzone = scene->items(fireZone);
        for(QGraphicsItem* item:itemsinzone){
            if(item->data(0)=="Enemy"){
                Enemy* ene = dynamic_cast<Enemy*>(item);
                if(ene->data(1)=="halable")
                    ene->setDead(true);
            }
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
    if(event->key() == Qt::Key_V) isVPressed = 1;

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
    if(event->key() == Qt::Key_V) isVPressed = 0;

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
        if(item->data(0)=="Enemy"||item->data(0)=="Fireball"||item->data(0)=="Fire"){
        if(item->data(0)=="Fireball")
            qDebug()<<"Fireball";
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
    for(QGraphicsItem* item : items_hit){
        if(item->data(0)=="Enemy"||item->data(0)=="Fireball"||item->data(0)=="Fire")
        {

            if(item->data(0)=="Enemy"){
                Enemy* eney = dynamic_cast<Enemy*>(item);
                if(output_ememy!=nullptr){
                    *output_ememy=eney;
                    if((*output_ememy)->getType()!=1)//type!=Gordo
                        (*output_ememy)->setDead(true);
                }
            }
            if(item->data(0)=="Fireball"){
                Projectile* pro=dynamic_cast<Projectile*>(item);
                pro->self_destroy();
            }
                return true;

        }
    }
    return false;
}

void Kirby::handelEnemyinHitbox(QRectF box){
    QList<QGraphicsItem*>itemsinbox=scene->items(box);
    for(QGraphicsItem* item : itemsinbox)
    {
        if(item->data(0)=="Enemy"||item->data(0)=="Fireball"||item->data(0)=="Fire")
        {
            move=move_ground;
            hp--;
            state=state_normal;
            isAir=0;
            hurt_frames=0;
            if(item->data(1)=="halable")
            {
                Enemy* ene = dynamic_cast<Enemy*>(item);
                ene->setDead(true);
            }

            if(item->x() > x())
            {
                vx=-knockback_sped;
            }else
                vx=knockback_sped;
            vy=knockup_sped;

            if(item->data(0)=="Fireball"){
                Projectile* pro=dynamic_cast<Projectile*>(item);
                pro->self_destroy();
            }
            return ;
        }
    }
    return;
}

void Kirby::get_item(){
    QList<QGraphicsItem*> items=scene->collidingItems(this);
    for(QGraphicsItem* item:items){
        if(item->data(0)=="MaxTomato")
        {
            item->setVisible(false);
            hp=3;
        }
        if(item->data(0)=="life")
        {
            life++;
            item->setVisible(false);
        }
    }
}


