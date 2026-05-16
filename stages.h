#ifndef STAGES_H
#define STAGES_H

#include "gameobject.h"
#include <QString>

class Stages : public GameObject
{
public:
    Stages(QString img_path, QGraphicsScene *scene
           ,int x, int y);

};

#endif // STAGES_H
