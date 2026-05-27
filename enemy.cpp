#include "enemy.h"
#include <iostream>
#include <ctime>

Enemy::Enemy(QString img_path, QGraphicsScene* main_scene
             ,int spawnPoint_x, int spawnPoint_y, int glevel,bool isGordo,int scene_id

             ):GameObject(img_path,main_scene)
{
    id=scene_id;
    scene = main_scene;
    spaw_x = spawnPoint_x; spaw_y = spawnPoint_y;
   // std::cout<<"Cons X,Y"<<spaw_x<<' '<<spaw_y<<std::endl;
    setPos(spaw_x,spaw_y);
    setData(0,"Enemy");
    turning_x=x();

    //random enemy type, just for fun
    level=glevel;
    if(glevel == 1){
        type = rand()%2;
//std::cout<<"Type: "<<type<<std::endl;
    }else if(glevel == 2){
        //make sure every type of enemy exist
        if(!isGordo){
        bool is0=0, is1=0,is2=0, is3=0;
        for(int i =0;i<types.size();i++){
            if(types[i]==0) is0=1;
            if(types[i]==1) is1=1;
            if(types[i]==2) is2=1;
            if(types[i]==3) is3=1;
        }
        if(!is0)type = 0;
        else if(!is1)type = 1;
        else if(!is2)type = 2;
        else if(!is3)type = 3;
        else type = rand()%4;
        types.append(type);
    }else type=Gordo;//Gordo is cool
    types.append(type);
    }//std::cout<<"type: "<<type;
    if(type==Gordo)
        setData(1,"Inhalable");
    else setData(1,"halable");
    setImg();


}
QList<int> Enemy::types={-1};

Enemy::~Enemy(){
    for(int i =0;i<types.size();i++)
        types[i] = -1;
};
void Enemy::update(){

    if(type==Sparky)
    {
        spark_movement();
    }
    else if((id==12||id==10)&&level==scene_2){
        updownMove();
    }
    else {
        movement();
        temp_y=y();
        Turn();
        while(isBlocked()){
            setY(y()-5);
        }
        if(isYWeird()){
            if(y()-temp_y>50||y()-temp_y<50){
                setY(temp_y);
            }
        }
    }


}

void Enemy::setImg(){
    switch(type){
        case Waddle_Dee:
            setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_0.png"));
            break;
        case Gordo:
            setPixmap(QPixmap(":/Image/Gordo/Gordo(0).png").scaled(74,74,Qt::KeepAspectRatio));
            break;
        case Hot_Head:
            setPixmap(QPixmap(":/Image/Hot_Head/Hot_head_run_L.png"));
            break;
        case Sparky:
            setPixmap(QPixmap(":/Image/Sparky/Sparky_left_1.png").scaled(148,148,Qt::KeepAspectRatio));
            break;
    }
}

void Enemy::movement(){
    frame++;
    if(facing==right){
        setX(x()+1);
        switch(type){
            case Waddle_Dee:
            if(frame<img_frame){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_3.png"));
            }else if(frame<img_frame*2){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_4.png"));
            }else if(frame<img_frame*3){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_5.png"));
            }else if(frame<img_frame*4){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_6.png"));
            }else frame=0;
            break;
        case Gordo:
            if(frame<img_frame){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(0).png").scaled(74,74,Qt::KeepAspectRatio));
            }else if(frame<img_frame*2){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(1).png").scaled(74,74,Qt::KeepAspectRatio));
            }else frame=0;
            break;
        case Hot_Head:
            if(frame<img_frame){
                setPixmap(QPixmap(":/Image/Hot_Head/Hot_head_run_R.png"));
            }else if(frame<img_frame*2){
                setPixmap(QPixmap(":/Image/Hot_Head/Hot_head_run(2)_R.png"));
            }else frame=0;
            break;

        }
    }else if(facing==left){
        setX(x()-1);
        switch(type){
            case Waddle_Dee:
            if(frame<img_frame){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_0.png"));
            }else if(frame<img_frame*2){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_1.png"));
            }else if(frame<img_frame*3){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_2.png"));
            }else frame=0;
            break;
        case Gordo:
        if(frame<img_frame){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(0).png").scaled(74,74,Qt::KeepAspectRatio));
        }else if(frame<img_frame*2){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(1).png").scaled(74,74,Qt::KeepAspectRatio));
        }else frame=0;
        break;
        case Hot_Head:
            if(frame<img_frame){
                setPixmap(QPixmap(":/Image/Hot_Head/Hot_head_run_L.png"));
            }else if(frame<img_frame*2){
                setPixmap(QPixmap(":/Image/Hot_Head/Hot_head_run(2)_L.png"));
            }else frame=0;
            break;
       }
     }
}

void Enemy::updownMove(){
    if(y()>spaw_y){
        vfacing=up;
    }else if(y()<spaw_y-200){
        vfacing=down;
    }

    if(vfacing==up){
        setY(y()-0.5);
    }else if(vfacing==down){
        setY(y()+0.5);
    }
    frame++;
    if(frame<img_frame){
        setPixmap(QPixmap(":/Image/Gordo/Gordo(0).png").scaled(74,74,Qt::KeepAspectRatio));
    }else if(frame<img_frame*2){
        setPixmap(QPixmap(":/Image/Gordo/Gordo(1).png").scaled(74,74,Qt::KeepAspectRatio));
    }else frame=0;
}

void Enemy::spark_movement(){
   // std::cout<<isGrounded<<std::endl;
    //deal x coordinate first

    //sparkkkkk
    if(attk_timer>=180&&isSolid()){
        isAttacking=1;
        attk_timer=0;
    }else if(isAttacking){
        Attk();
    }else
    {
        attk_timer++;
        const double jump_x_spd=5;//the speed parallel to x
    if(!isGrounded){
        temp_x=x();
        if(facing==right)
            setX(x()+jump_x_spd);
        if(facing==left)
            setX(x()-jump_x_spd);
    }
    if(id==4){
        if(facing==right){
            if(x()>6000){
                facing = left;setX(x()-10); turning_x=x();
            }
        }else if(facing==left){
            if(x()<5652){
                facing = right;setX(x()+10); turning_x=x();
            }
        }
    }else if(id==7){
        if(facing==right){
            if(x()>7622){
                facing = left;setX(x()-10); turning_x=x();
            }
        }else if(facing==left){
            if(x()<7162){
                facing = right;setX(x()+10); turning_x=x();
            }
        }
    }else if(id==11){
        if(facing==right){
            if(x()>5543){
                facing = left;setX(x()-10); turning_x=x();
            }
        }else if(facing==left){
            if(x()<5033){
                facing = right;setX(x()+10); turning_x=x();
        }
        }

    }else if(id==6){
        if(facing==right){
            if(x()>4780){
                facing = left;setX(x()-10); turning_x=x();
            }
        }else if(facing==left){
            if(x()<4112){
                facing = right;setX(x()+10); turning_x=x();
            }
        }
    } else Turn();


    //deal y direction
    vy+=(vy>20)?0:gravity;
    setY(y()+vy);

    handelCollisionY();

    if(isGrounded){
        jump_timer++;

        if(facing==right)
            setPixmap(QPixmap(":/Image/Sparky/Sparky_ritht_2.png").scaled(148,148,Qt::KeepAspectRatio));
        else if(facing==left)
            setPixmap(QPixmap(":/Image/Sparky/Sparky_left_2.png").scaled(148,148,Qt::KeepAspectRatio));

        if(jump_timer>jump_interval){
            isGrounded=0;
            vy=jump_speed;
            jump_timer=0;
        }
    }else {
        if(facing==right)
            setPixmap(QPixmap(":/Image/Sparky/Sparky_ritht_1.png").scaled(148,148,Qt::KeepAspectRatio));
        else if(facing==left)
            setPixmap(QPixmap(":/Image/Sparky/Sparky_left_1.png").scaled(148,148,Qt::KeepAspectRatio));
    }
    }

}

void Enemy::Turn(){
    if(id==7&&level==scene_2)
        {
            if(facing==right)
            {
                if(x()>(7622-pixmap().width())){
                    facing = left;setX(x()-10); turning_x=x();
                }
            }else if(facing==left)
            {
                if(x()<7162){
                    facing = right;setX(x()+10); turning_x=x();
                }
            }
        }else if(id==11&&level==scene_2)
        {
            if(facing==right)
            {
                if(x()>5543)
                {
                    facing = left;setX(x()-10); turning_x=x();
                }
            }else if(facing==left)
            {
                if(x()<5033)
                {
                    facing = right;setX(x()+10); turning_x=x();
                }
            }
        }else if(id==3&&level==scene_1)
        {
            if(facing==right)
            {
                if(x()>2600)
                {
                    facing = left;setX(x()-10); turning_x=x();
                }
            }else if(facing==left)
            {
                if(x()<2166)
                {
                    facing = right;setX(x()+10); turning_x=x();
                }
            }
        }else if(id==5&&level==scene_1)
        {
            if(facing==right)
            {
                if(x()>3880)
                {
                    facing = left;setX(x()-10); turning_x=x();
                }
            }else if(facing==left)
            {
                if(x()<3708)
                {
                    facing = right;setX(x()+10); turning_x=x();
                }
            }
        }else if(type == Hot_Head)/*the image of hot heed is weird when turning,
                                   so i move a little bit more than others*/
        {
            if(isWallBlocked())
            {
                if(facing==right) {facing=left; setX(x()-20);turning_x=x();}
                else {facing=right;setX(x()+20);turning_x=x();}
            }else
            {
                if(facing==right)
                {
                    if(x()-turning_x > turning_distance)
                    {
                        facing = left;setX(x()-25); turning_x=x();
                    }
                }else if(facing==left){
                    if(turning_x-x() > turning_distance){
                        facing = right;setX(x()+25); turning_x=x();
                    }
                }
            }
        }else if(isWallBlocked())
        {
            if(facing==right) {facing=left; setX(x()-20);turning_x=x();}
            else {facing=right;setX(x()+20);turning_x=x();}
        }

}


//blocked by terrain in x
bool Enemy::isWallBlocked(){
    QList<QGraphicsItem*> items_hit = scene->collidingItems(this);
    for(QGraphicsItem* item:items_hit){
        if(item->data(0)=="Solid")
        {
            //if the item is lower than enemy, then it's ground
            if(type==Sparky){
                if(item->y() > (this->y()+this->pixmap().height()-20)){
                    continue;
                }
            }
            else{
                if(item->y() > (this->y()+this->pixmap().height()-5)){
                    continue;
                }
            }

            return true;
        }
    }
    return 0;
}

//Only for initialization
bool Enemy::isBlocked(){
    QList<QGraphicsItem*> items_hit = scene->collidingItems(this);
    for(QGraphicsItem* item:items_hit){
        if(item->data(0)=="Solid")
        {
            return 1;
        }
    } return 0;
}

bool Enemy::isYWeird(){
    if(y()-temp_y>50||y()-temp_y<-50){
        return true;
    }
    return false;
}

void Enemy::handelCollisionY(){
    QList<QGraphicsItem*> items_hit = scene->collidingItems(this);
    for(QGraphicsItem* item:items_hit){
        if(item->data(0)=="Solid"){
            if(vy>0){
                while(isBlocked()){
                    setY(y()-1);
                }
                isGrounded=1;
                vy=0;
                break;
            }
        }
    }
}

bool Enemy::isSolid(){
    QList<QGraphicsItem*> items_hit = scene->collidingItems(this);
    for(QGraphicsItem* item:items_hit){
        if(item->data(0)=="Solid"){
            return true;
        }
    }
    return false;

}

void Enemy::setDead(bool dead){
   isdead=dead;
   if(dead)
   {
    setVisible(false);
   }
}

void Enemy::respawn(){
    setVisible(true);
    isdead=false;
    setPos(spaw_x,spaw_y);
    turning_x = spaw_x;
}

//i think its only for hot heed
bool Enemy::isPlayerNear(){
    QRectF fireZone;
    if (facing == right)
    {
        fireZone = QRectF(x() + pixmap().width(), y(), 500, pixmap().height());

    } else {
        fireZone = QRectF(x() , y(), 500, pixmap().height());
    }

    QList<QGraphicsItem*> items_in_zone = scene->items(fireZone);
    for(QGraphicsItem* item : items_in_zone)
    {
        if(item->data(0)=="Player")
        {
            return true;
        }
    }
    return false;

}






void Enemy::Attk()
{
    frame++;
    switch (type) {
        case Hot_Head:
        break;
        case Sparky:
            if(frame<img_frame){
                setOffset(0,-10);
                setPixmap(QPixmap(":/Image/Sparky/Sparky_attack_1.png").scaled(128,128,Qt::KeepAspectRatio));
            }else if(frame<img_frame*6){
                setOffset(-108,-108);
                setPixmap(QPixmap(":/Image/Sparky/Sparky_attack_2.png").scaled(342,342,Qt::KeepAspectRatio));
            }else {
                setOffset(0,0);
                setPixmap(QPixmap((facing==right)?":/Image/Sparky/Sparky_ritht_1.png":":/Image/Sparky/Sparky_left_1.png").scaled(148,148,Qt::KeepAspectRatio));
                attk_timer=0;frame=0;isAttacking=0;
            }
        break;
    default://other than Sparky&HotHeed
        break;
    }
}



