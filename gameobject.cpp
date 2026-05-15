#include "gameobject.h"

GameObject::GameObject(QString img_path, QGraphicsScene *scene)
{
    setPixmap(QPixmap(img_path));

    setPos(0,0);
    scene ->addItem(this);
}
