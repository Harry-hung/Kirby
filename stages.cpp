#include "stages.h"

Stages::Stages(QString path, QGraphicsScene *scene) : GameObject(path, scene)
{
    setPos(0,1080-pixmap().height());
}
