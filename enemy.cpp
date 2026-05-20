#include "enemy.h"
#include <iostream>
Enemy::Enemy(QString img_path, QGraphicsScene* main_scene
             ,int spawnPoint_x, int spawnPoint_y, int level,bool isGordo=0):GameObject(img_path,main_scene)
{

    scene = main_scene;
    spaw_x = spawnPoint_x; spaw_y = spawnPoint_y;
   // std::cout<<"Cons X,Y"<<spaw_x<<' '<<spaw_y<<std::endl;
    setPos(spaw_x,spaw_y);
    setData(0,"Enemy");
    //random enemy type, just for fun
    if(!isGordo){
    if(level == 1){
        type = rand()%2;
    }else if(level == 2){
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
    }
    }else type=Gordo;//Gordo is cool
//std::cout<<"type: "<<type;
    setImg();
}

QList<int> Enemy::types={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

Enemy::~Enemy(){
    for(int i =0;i<types.size();i++)
        types[i] = -1;
};
void Enemy::update(){
    movement();
    if(isBlocked()){
        handelCollision();}

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
        switch(type){
            case Waddle_Dee:
            setX(x()+1);
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
            setX(x()+1);
        if(frame<img_frame){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(0).png").scaled(74,74,Qt::KeepAspectRatio));
        }else if(frame<img_frame*2){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(1).png").scaled(74,74,Qt::KeepAspectRatio));
        }else frame=0;
        break;

        }
    }else if(facing==left){
        switch(type){
            case Waddle_Dee:
            setX(x()-1);
            if(frame<img_frame){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_0.png"));
            }else if(frame<img_frame*2){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_1.png"));
            }else if(frame<img_frame*3){
                setPixmap(QPixmap(":/Image/Waddle_Dee/Waddle_Dee_2.png"));
            }else frame=0;
            break;
        case Gordo:
            setX(x()-1);
        if(frame<img_frame){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(0).png").scaled(74,74,Qt::KeepAspectRatio));
        }else if(frame<img_frame*2){
            setPixmap(QPixmap(":/Image/Gordo/Gordo(1).png").scaled(74,74,Qt::KeepAspectRatio));
        }else frame=0;
        break;
       }
     }
}

//blocked by terrain
bool Enemy::isBlocked(){
    QList<QGraphicsItem*> items_hit = scene->collidingItems(this);
    for(QGraphicsItem* item : items_hit){
        if(item -> data(0)=="Solid"){
            return 1;
        }
    }
    return 0;
}

void Enemy::handelCollision(){
    double tempy=y();
    int height=0;
    while(isBlocked()&&height<5){
        setY(y()-1);
        height++;
    }
    if(isBlocked()) setY(tempy);
        else return;
    if(facing==right) {facing=left; setX(x()-10);}
    else {facing=right;setX(x()+10);}
}
