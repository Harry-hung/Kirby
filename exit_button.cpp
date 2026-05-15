#include "exit_button.h"

Buttons::Buttons(QString path, QGraphicsScene *scene)
{
    setPixmap(QPixmap(path).scaled(50,50,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    setZValue(50);
    setPos(1610-pixmap().width(),0);
    setOpacity(0.7);
    scene -> addItem(this);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

void Buttons::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(contains(event->pos()))
        isButtonPress = 1;
    else isButtonPress = 0;

    isButtonRelease = 0;
}

void Buttons::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(contains(event->pos())){
        game_closing = 1;
        isButtonRelease = 1;
    }else isButtonRelease = 1;

    isButtonPress = 0;
}

void Buttons::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    isHoverEnter = 1;
    isHoverLeave = 0;
}

void Buttons::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    isHoverLeave = 1;
    isHoverEnter = 0;
}

void Buttons::update(){
    if(isButtonPress){
        setOpacity(1);
        setScale(1.1);
    }else if(game_closing){
        QApplication::quit();
    }else if(isButtonRelease){
        setOpacity(0.7);
        setScale(1);
        isButtonRelease = 0;
    }else if(isHoverEnter){
        setOpacity(0.9);
        setScale(1.05);
    }else if(isHoverLeave){
        setOpacity(0.7);
        setScale(1);
    }else {//Default just in case
        setOpacity(0.7);
        setScale(1);
    }
}
