#ifndef STAGES_H
#define STAGES_H

#include "gameobject.h"
#include <QString>

class Stages : public GameObject
{
public:
    Stages(QString img_path, QGraphicsScene *scene
           ,int x, int y);

private:

    //stage type
    enum stage_type{
        stage1_1 = 1, stage1_2 ,
             stage1_3 , stage2_1 , stage2_2
    };
};

#endif // STAGES_H
