#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QString>

class GameObject : public QGraphicsPixmapItem
{
public:
    GameObject(QString path, QGraphicsScene *scene);
    void setCollisionShape();

private:
    QPainterPath
};

#endif // GAMEOBJECT_H
