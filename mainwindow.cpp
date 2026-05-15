#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setScene();
    ui->setupUi(this);
    gameTimer = new QTimer(this);
    exit_button = new Buttons(":/Image/exitwindowbutton.png",Scene);
    player = new Kirby(":/Image/Kirby_normal/kirby_stop_R.png",Scene);
    stage = new Stages(":/Image/background/Stage1(1).png",Scene);
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

    //player movement
    if(player){
        player ->updateMovement(4);
        QPointF playerPos_onScene = player->mapToScene(0,0);
        view->centerOn(playerPos_onScene.x()+50,540);
        // advanced view->centerOn(player->x() + 200, view->sceneRect().center().y());
    }else qDebug()<<"Failed to load Kirby";


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

void MainWindow::setScene(){//set scene
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

    if(game_scene==scene_1||game_scene==scene_2){
        if(!player)
            player = new Kirby(":/Image/Kirby_normal/kirby_stop_R.png",Scene);
    }else if(game_scene==scene_start){
        player = nullptr;
    }
    if(!exit_button)
        exit_button = new Buttons(":/Image/exitwindowbutton.png",Scene);
}









