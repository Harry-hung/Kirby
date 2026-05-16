#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainterPath>
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
    void setCollisionShape(QString json_path);

    QPainterPath shape() const override;
private:
    void jsonToPath(QString json_path);
    QPainterPath m_customPath ;
};

#endif // GAMEOBJECT_H
