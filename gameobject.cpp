#include "gameobject.h"

GameObject::GameObject(QString img_path, QGraphicsScene *scene)
{
    setPixmap(QPixmap(img_path));

    setPos(0,0);

}

//call jsonToPath to read the path for shape
void GameObject::setCollisionShape(QString json_path){
    jsonToPath(json_path);
    return;
}

QPainterPath GameObject::shape() const{
    if(m_customPath.isEmpty()){
        return QGraphicsPixmapItem::shape();
    }else return m_customPath;

}

//read the object json file into path for the collision shape
void GameObject::jsonToPath(QString json_path){
    QFile file(json_path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Fail to open object json file "<<json_path;
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray objects = doc.object()["tiles"].toArray()[0].toObject()["objectgroup"].toObject()["objects"].toArray();
    double shift_x = objects[0].toObject()["x"].toDouble();
    double shift_y = objects[0].toObject()["y"].toDouble();

    QJsonArray points = objects[0].toObject()["polygon"].toArray();
    QPainterPath path;
    path.moveTo(shift_x, shift_y);
    for(int i=0; i < points.size();i++){
        double rel_x = points[i].toObject()["x"].toDouble();
        double rel_y = points[i].toObject()["y"].toDouble();
        path.lineTo((shift_x + rel_x)
                    ,(shift_y + rel_y));
    }
    path.closeSubpath();
    m_customPath = path;

}
