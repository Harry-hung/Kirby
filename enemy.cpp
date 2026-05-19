#include "enemy.h"
#include <iostream>
Enemy::Enemy(QString img_path, QGraphicsScene* main_scene
             ,int spawnPoint_x, int spawnPoint_y, int level):GameObject(img_path,main_scene)
{
    scene = main_scene;
    spaw_x = spawnPoint_x; spaw_y = spawnPoint_y;
   // std::cout<<"Cons X,Y"<<spaw_x<<' '<<spaw_y<<std::endl;
    setPos(spaw_x,spaw_y);
    setData(0,"Enemy");
    //random enemy type, just for fun
    if(level == 1){
        type = rand()%2;
    }else if(level == 2){
        type = rand()%4;
    }
    setImg();
}

void Enemy::update(){
    movement();
    frame++;
}

void Enemy::setImg(){
    switch(type){
        case Waddle_Dee:
            setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_0.png"));
            break;
        case Gordo:
            setPixmap(QPixmap(":/Image/Gordo/Gordo(0).png"));
            break;
        case Hot_Head:
            setPixmap(QPixmap(":/Image/Hot_Head/Hot_head_run_L.png"));
            break;
        case Sparky:
            setPixmap(QPixmap(":/Image/Sparky/Sparky_left_1.png"));
            break;
    }
}

void Enemy::movement(){

    if(facing==right){
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
            }frame=0;
            break;
        case Gordo:
        if(frame<img_frame){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(0).png"));
        }else if(frame<img_frame*2){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(1).png"));
        }frame=0;
        break;

        }
    }else if(facing==left){
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
            setPixmap(QPixmap(":/Image/Gordo/Gordo(0).png"));
        }else if(frame<img_frame*2){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(1).png"));
        }else frame=0;
        break;
       }
     }
}

//blocked by terrain
bool Enemy::isBlocked(){
    QList<QGraphicsItem*> items_hit = scene->collidingItems(this);
    for(QGraphicsItem* item : items_hit){
        if(item -> data(0)=="Stage"){
            return 1;
        }
    }
    return 0;
}
