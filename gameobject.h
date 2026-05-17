#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QPainter>
#include <QString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include <QDebug>

class GameObject : public QGraphicsPixmapItem
{
public:
    GameObject(QString path, QGraphicsScene *scene);

    void paint(QPainter* painter,const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:

};

#endif // GAMEOBJECT_H
