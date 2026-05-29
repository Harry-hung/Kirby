#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QBrush>
#include <QString>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QPixmap>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QtNumeric>

#include "kirby.h"
#include "enemy.h"
#include "exit_button.h"
#include "platform.h"
#include "kirby.h"
#include "exit_button.h"
#include "stages.h"
#include "door.h"
#include "projectile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setBG(QString background_path);
    void setView();
    void setScene();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void switchScene();
    void Item_Default();
    void loadTiledMap(QString json_path);

    bool eventFilter(QObject *obj, QEvent *event) override;
    bool isPlayerGround();

    void projUpdate();

    void kirby_status();
    void kirby_init();

private slots:
    void gameUpdate();

private:
    QList<QLabel*> hearts;
    QLabel* lives;
    QLabel* lives_Label;//the number of remain lives

    QLabel* state_Label;
    const double state_img_height=150;
    const double state_img_width =112.5;

    QGraphicsView *view;
    Ui::MainWindow *ui;
    QGraphicsScene *Scene;
    QTimer *gameTimer;
    Kirby *player;
    Buttons *exit_button;
    QList<Enemy*> enemys;


    //scene 1,2,3
    const static int scene_start =0,
        scene_1 = 1,scene_2 = 2,
        scene_over = 3, scene_clear = 4;
    int game_scene = scene_start;

};
#endif // MAINWINDOW_H
