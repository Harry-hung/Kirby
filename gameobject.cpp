#include "gameobject.h"

GameObject::GameObject(QString img_path, QGraphicsScene *scene)
{
    setPixmap(QPixmap(img_path));

    setPos(0,0);

}

//call jsonToPath to read the path for shape
//void GameObject::setCollisionShape(QString json_path){
//    jsonToPath(json_path);
//    return;
//}


/*
void GameObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsPixmapItem::paint(painter, option, widget);

            // 設定畫筆：紅色、粗細為 2 像素
            painter ->setPen(QPen(Qt::red,2));
            // 畫出真正的碰撞箱！
            // 如果你是用預設的方形碰撞，用 drawRect：


            // 如果你未來有覆寫 shape() 變成多邊形，就用 drawPath：
            painter->drawPath(shape());

}
*/

