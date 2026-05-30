#include "projectile.h"

Projectile::Projectile(QGraphicsScene* game_scene,
        double spawn_x, double spawn_y,QString target_name,int player_facing, QString bullet_type):spaw_x(spawn_x),
  spaw_y(spawn_y),GameObject("",game_scene),target(target_name),scene(game_scene),facing(player_facing),type(bullet_type)
{
    frame=0;
    setPos(spaw_x,spaw_y);
    setZValue(4);
    if(target_name=="Player") setData(0,"Fireball");
    if(target_name=="Enemy") setData(0,"Bullet");
    if(type=="Fireball") setPixmap(QPixmap(":/Image/Hot_Head/hotheed_fireball.png"));
    if(type=="Gas") setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/air_R.png":":/Image/Kirby_normal/air_L.png").scaledToHeight(100));
    if(type=="Star")setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/star(2)_R.png":":/Image/Kirby_normal/star(2)_L.png").scaledToHeight(100));

}
QPainterPath Projectile::shape() const
{
    QPainterPath path;
    if(type=="Fireball") path.addRect(10,10, pixmap().width()-20, pixmap().height()-20);
    else if(type=="Star") path.addRect(23,23, pixmap().width()-46, pixmap().height()-46);
    else path.addRect(15,15, pixmap().width()-30, pixmap().height()-30);

    return path;
}
void Projectile::update()
{
    if(facing==right) setX(x()+10);
    if(facing==left) setX(x()-10);

    if(x()>scene->width()||x()<0)
    {
        self_destroy();
        return;
    }

    if(hit()){
        self_destroy();
        return;
    }

    if(type=="Fireball"){
        Fireball();
    }else if(type=="Star")Star();
    else if(type=="Gas")Gas();


}

void Projectile::self_destroy()
{
    if(scene)
    {
        scene->removeItem(this);
    }

    delete this;
}

bool Projectile::hit()
{
    QList<QGraphicsItem*>items = scene->collidingItems(this);

    for(QGraphicsItem* item:items)
    {
        if(item->data(0)=="Solid")
        {
            return true;
        }
        if(item->data(0)=="Enemy"&&target=="Enemy")
        {
                Enemy* enemy = dynamic_cast<Enemy*>(item);
                if(item->data(1)!="Inhalable") enemy->setDead(true);//Not Gordo
                return true;

        }
        if(item->data(0)=="Player")
        {
            //Player hit is handel by player
        }
    }
    return false;
}


void Projectile::Star()
{
    frame++;
    if(frame<15){
        setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/star_R.png":":/Image/Kirby_normal/star_L.png").scaledToHeight(130));
    }else if(frame<30){
        setPixmap(QPixmap((facing==right)?":/Image/Kirby_normal/star(3)_R.png":":/Image/Kirby_normal/star(3)_L.png").scaledToHeight(130));
    }else frame = 0;
}

void Projectile::Gas()
{
    if(std::abs(x()-spaw_x)>100) self_destroy();
}

void Projectile::Fireball()
{
    //Nothing
}



