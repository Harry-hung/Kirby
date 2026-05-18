#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setScene();
    ui->setupUi(this);
    gameTimer = new QTimer(this);
    exit_button = new Buttons(":/Image/exitwindowbutton.png",Scene);
    setBG(":/Image/background/start.png");
    setView();
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::gameUpdate);
    gameTimer ->start(16);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::gameUpdate(){
    if(game_scene != scene_start){
        //player movement
        if(player){

            player ->updateMovement(4);
            QPointF playerPos_onScene = player->mapToScene(0,0);
            view->centerOn(playerPos_onScene.x()+50,540);

        }//else qDebug()<<"Failed to load Kirby";
    }

    //button
    if(exit_button){
        exit_button ->update();
        exit_button ->setPos(view->mapToScene(1610,0).x() - exit_button->pixmap().width(),0);
    }
}

//set background
void MainWindow::setBG(QString path){

    QPixmap Bg1(path);
    QPixmap scaledBg1;
        if(game_scene==scene_start)
            scaledBg1 = Bg1.scaled(1620,1080,Qt::IgnoreAspectRatio);
        else if(game_scene == scene_1)
            scaledBg1 = Bg1.scaled(4860,1080,Qt::IgnoreAspectRatio);
        else if(game_scene==scene_2)
            scaledBg1 = Bg1.scaled(8100,1080,Qt::IgnoreAspectRatio);

        QGraphicsPixmapItem *bg1Item = new QGraphicsPixmapItem(scaledBg1);
        bg1Item ->setPos(0,0);
        bg1Item ->setZValue(-1);

        Scene->addItem(bg1Item);


/*
    if(game_scene ==scene_1||game_scene==scene_2){
        QPixmap Bg2(path);
        QPixmap Bg3(path);
            QPixmap scaledBg2 = Bg2.scaled(1620,1080,Qt::IgnoreAspectRatio);
            QPixmap scaledBg3 = Bg3.scaled(1620,1080,Qt::IgnoreAspectRatio);

            QGraphicsPixmapItem *bg2Item = new QGraphicsPixmapItem(scaledBg2);
            QGraphicsPixmapItem *bg3Item = new QGraphicsPixmapItem(scaledBg3);

            bg2Item ->setPos(1620,0);
            bg3Item ->setPos(3240,0);
            bg2Item ->setZValue(-1);
            bg3Item ->setZValue(-1);

            Scene->addItem(bg2Item);
            Scene->addItem(bg3Item);

        if(game_scene == scene_2){
            QPixmap Bg4(path);
            QPixmap Bg5(path);

                QPixmap scaledBg4 = Bg4.scaled(1620,1080);
                QPixmap scaledBg5 = Bg5.scaled(1620,1080);

                QGraphicsPixmapItem *bg4Item = new QGraphicsPixmapItem(scaledBg4);
                QGraphicsPixmapItem *bg5Item = new QGraphicsPixmapItem(scaledBg5);

                bg4Item ->setPos(4860,0);
                bg5Item ->setPos(6480,0);
                bg4Item ->setZValue(-1);
                bg5Item ->setZValue(-1);

                Scene->addItem(bg4Item);
                Scene->addItem(bg5Item);

        }
    }*/

}

void MainWindow::setView(){//set view
    view = new QGraphicsView(Scene, this);
    view->setFocusPolicy(Qt::NoFocus);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(1620,1080);
    setCentralWidget(view);
}

void MainWindow::setScene(){//set Start scene
    Scene = new QGraphicsScene(this);
    Scene->setSceneRect(0,0,1620,1080);
}



void MainWindow::keyPressEvent(QKeyEvent *event){
    if(player)
        player -> handlePressEvent(event);
    if(event->isAutoRepeat())
        return;
    if(game_scene == scene_start && event->key() == Qt::Key_Return){
        game_scene = scene_1;
        switchScene();
    }else if(game_scene == scene_1 && event->key() == Qt::Key_Return){
        game_scene = scene_2;
        switchScene();
    }
    return;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if(player)
        player -> handleReleaseEvent(event);

    return;
}

void MainWindow::switchScene(){
    switch (game_scene){
    case scene_start:
        Scene->setSceneRect(0,0,1620,1080);
        setBG(":/Image/background/start.png");
        break;
    case scene_1:
        Scene -> clear();
        Scene ->setSceneRect(0,0,4860,1080);
        Item_Default();
        loadTiledMap(":/scene_1.json");
        player->setY(player->y()-60);
        setBG(":/Image/background/Background.jpg");
        break;
    case scene_2:
        Scene->clear();
        Scene->setSceneRect(0,0,8100,1080);
        Item_Default();
        setBG(":/Image/background/Background.jpg");
        break;
    }
}

void MainWindow::Item_Default(){
    player =nullptr;
    exit_button = nullptr;
    if(!exit_button)
        exit_button = new Buttons(":/Image/exitwindowbutton.png",Scene);

}



void MainWindow::loadTiledMap(QString json_path){
    player = nullptr;
    QFile file(json_path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Fail to find json file";
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject mapdata = doc.object();

    int mapWidth = mapdata["width"].toInt();

    //object GID
    int stage1_1=10, stage1_2=10,stage1_3=10
            ,floor=10,door=10;

    //get GID of every tile
    QJsonArray tilesets = mapdata["tilesets"].toArray();
    for(int i=0;i<tilesets.size();i++){
        QString tilename = tilesets[i].toObject()["source"].toString();
        if(tilename == "Stage1(1).tsx")
            stage1_1 = tilesets[i].toObject()["firstgid"].toInt();
        if(tilename == "Stage1(2).tsx")
            stage1_2 = tilesets[i].toObject()["firstgid"].toInt();
        if(tilename == "Stage1(3).tsx")
            stage1_3 = tilesets[i].toObject()["firstgid"].toInt();
        if(tilename == "floor.tsx")
            floor = tilesets[i].toObject()["firstgid"].toInt();
        if(tilename == "door.tsx")
            door = tilesets[i].toObject()["firstgid"].toInt();

    }

    QJsonArray layers = mapdata["layers"].toArray();
    for(int i=0;i<layers.size();i++){
        if(layers[i].toObject()["type"] == "tilelayer"){
            QJsonArray data = layers[i].toObject()["data"].toArray();
            for(int j=0;j<data.size();j++){
                if(data[j]==0)
                    continue;
                else{
                    int x = (j % mapWidth);
                    int y = (j / mapWidth);
                    if(data[j] == stage1_1){
                        Stages* stage = new Stages(":/Image/background/Stage1(1).png",
                                                   Scene, 0, y);
                        stage->setPos(0, y-stage->pixmap().height()+60);
                        Scene->addItem(stage);
                    }
                    if(data[j] ==stage1_2){
                        QPixmap img(":/Image/background/Stage1(2).png");
                        QPixmap scaled_img = img.scaled(img.width()+100,img.height()+100,Qt::IgnoreAspectRatio);
                        Stages* stage = new Stages(":/Image/background/Stage1(2).png",
                                                   Scene, x, y);
                        stage->setPos(x, y-stage->pixmap().height()+60);
                        stage->setPixmap(scaled_img);
                        Scene->addItem(stage);
                    }
                    if(data[j] == stage1_3){
                        Stages* stage = new Stages(":/Image/background/Stage1(3).png",
                                            Scene, x, y);
                        stage->setPos(x, y -(stage->pixmap().height())+60);
                        Scene->addItem(stage);
                    }
                    if(data[j]== floor){
                        std::cout<<"floor";
                        Scene->addItem(new Platform(":/Image/item/floor.png",Scene,x,y));
                    }
                    if(data[j] == door){
                        Door* portal = new Door(":/Image/item/door.png",Scene);
                        portal->setPos(x,y-(portal->pixmap().height())+120);
                        Scene->addItem(portal);
                    }
                }
            }
        }else if(layers[i].toObject()["type"] == "objectgroup"){
            QJsonArray objects = layers[i].toObject()["objects"].toArray();
            for(int j=0;j<objects.size();j++){
                if(objects[j].toObject()["name"] == "PlayerSpawn"){
                    if(!player){
                        player = new Kirby(":/Image/Kirby_normal/kirby_stop_R.png",Scene);
                        Scene ->addItem(player);
                    }
                    int x = objects[j].toObject()["x"].toInt();
                    int y = objects[j].toObject()["y"].toInt();

                    player->setPos(x,y);

                }else if(objects[j].toObject()["name"] == "EnemySpawn"){

                }
            }

        }
    }

}










