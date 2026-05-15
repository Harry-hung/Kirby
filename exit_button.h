#ifndef EXIT_BUTTON_H
#define EXIT_BUTTON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>

class Buttons : public QGraphicsPixmapItem
{
public:
    Buttons(QString img_path, QGraphicsScene *scene);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void update();
private:
    int but_x, but_y;

    bool isButtonPress = 0;
    bool isButtonRelease = 0;
    bool isHoverEnter = 0;
    bool isHoverLeave = 0;

    //only for exit button
    bool game_closing = 0;
};

#endif // EXIT_BUTTON_H
