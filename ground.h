#ifndef GROUND_H
#define GROUND_H

#include "gameobject.h"

class Ground : public GameObject
{
public:
    Ground(QString path, QGraphicsScene *, int x, int y);
};

#endif // GROUND_H
