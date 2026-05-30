#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setScene();
    player=nullptr;
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
    if(game_scene != scene_start&&game_scene!=scene_clear&&game_scene!=scene_over){
        //player movement
        if(player){
            if(player->life==0) {
                game_scene=scene_over;
                switchScene();
                return;
            }
            player ->updateMovement(4);
            kirby_status();
            QPointF playerPos_onScene = player->mapToScene(0,0);
            view->centerOn(playerPos_onScene.x()+50,540);

            //Enemy Respawn
            for(Enemy* enemy : enemys){
                if(enemy->isDead()){
                    double d_kirby=qAbs(enemy->x()-player->x());
                    if(d_kirby>1500)
                    {
                        enemy->respawn();
                    }
                }
            }

            //Kirby through door
            if(player->isNextScene()){
                if(game_scene==scene_1)game_scene = scene_2;
                else if(game_scene==scene_2)game_scene = scene_clear;

                switchScene();
                return;
            }

        }else qDebug()<<"Failed to load Kirby";
        //projectile
        projUpdate();

        //Enemy
        if(!enemys.isEmpty())
        for(Enemy* enemy:enemys)
            enemy->update();
    }else if(game_scene==scene_over){
        sceneOver();
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
        else if(game_scene==scene_clear)
            scaledBg1 = Bg1.scaled(1620,1080,Qt::KeepAspectRatio);
        else if(game_scene==scene_over)
            scaledBg1 = Bg1.scaled(1620,1080,Qt::KeepAspectRatio);
        if(bg1Item==nullptr){
            bg1Item = new QGraphicsPixmapItem(scaledBg1);
            bg1Item ->setPos(0,0);
            bg1Item ->setZValue(-1);
            Scene->addItem(bg1Item);
        }else bg1Item ->setPixmap(scaledBg1);


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
    view->setDragMode(QGraphicsView::NoDrag);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(1620,1080);
    view->viewport()->installEventFilter(this);
    setCentralWidget(view);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 攔截滑鼠滾輪事件，避免畫面上下左右滑動
    if (event->type() == QEvent::Wheel) {
        return true; // 直接攔截，不讓 View 處理捲動
    }
    return QMainWindow::eventFilter(obj, event);
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
    }

    if(game_scene == scene_over&&!iscontinue&&event->key()==Qt::Key_Return) QApplication::quit();
    if(game_scene == scene_clear&&event->key() == Qt::Key_Return) QApplication::quit();
    if(game_scene == scene_over && iscontinue && event->key()==Qt::Key_Return)
        {
        game_scene = scene_start;
        switchScene();
    }
    if(event->key()==Qt::Key_Up)isUpKeyPressed=1;
    if(event->key()==Qt::Key_Down)isDownKeyPressed=1;

    return;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if(player)
        player -> handleReleaseEvent(event);
    if(event->isAutoRepeat())
        return;
    if(event->key()==Qt::Key_Up)isUpKeyPressed=0;
    if(event->key()==Qt::Key_Down)isDownKeyPressed=0;


    return;
}

void MainWindow::sceneOver(){
    state_Label->hide();
    lives->hide();
    lives_Label->hide();
    for(QLabel* heart:hearts)
        heart->hide();
    if(isUpKeyPressed&&!iscontinue&&isUpKeyReleased){
        iscontinue=1;
        isUpKeyReleased=0;
        setBG(":/Image/background/game_over_continue.png");
    }else if(isDownKeyPressed&&iscontinue&&isDownKeyReleased){
        iscontinue=0;
        isDownKeyReleased=0;
        setBG(":/Image/background/game_over_quit.png");
    }
    if(!isUpKeyPressed){
        isUpKeyReleased=1;
    }
    if(!isDownKeyPressed){
        isDownKeyReleased=1;
    }
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
        kirby_init();
   //  player->setX(4800);
        setBG(":/Image/background/Background.jpg");
        break;
    case scene_2:
        kirby_lives=player->life;
       // qDebug()<<kirby_lives;
        srand(time(NULL));
        Scene->clear();
        Scene->setSceneRect(0,0,8100,1080);
        Item_Default();
        loadTiledMap(":/scene_2.json");
      // player->setX(7500);
        kirby_init();
        setBG(":/Image/background/Background.jpg");
        break;
    case scene_clear:
       // std::cout<<"pp";
        Scene->clear();
        Scene->setSceneRect(0,0,1620,1080);
        Item_Default();
        kirby_init();
        setBG(":/Image/background/Clear.png");
        break;
    case scene_over:
        Scene->clear();
        Scene->setSceneRect(0,0,1620,1080);
        Item_Default();
        sceneOver();
        setBG(":/Image/background/game_over_continue.png");
        break;
    }
}

void MainWindow::Item_Default(){
    player =nullptr;
    exit_button = nullptr;
    bg1Item = nullptr;
    enemys.clear();
    if(!exit_button)
        exit_button = new Buttons(":/Image/exitwindowbutton.png",Scene);

}



void MainWindow::loadTiledMap(QString json_path){
    player = nullptr;
    QFile file(json_path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Fail to find map json file";
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject mapdata = doc.object();

    int mapWidth = mapdata["width"].toInt();

    //object GID
    int stage1_1=10, stage1_2=10,stage1_3=10
            ,stage2_2=-1, stage2_1=-1,floor=10,goal_door=-1,door=10,brick=-1;

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
        if(tilename == "Stage2(1).tsx")
            stage2_1 = tilesets[i].toObject()["firstgid"].toInt();
        if(tilename == "Stage2(2).tsx")
            stage2_2 = tilesets[i].toObject()["firstgid"].toInt();
        if(tilename == "floor.tsx")
            floor = tilesets[i].toObject()["firstgid"].toInt();
        if(tilename == "door.tsx")
            door = tilesets[i].toObject()["firstgid"].toInt();
        if(tilename == "goal_door.tsx")
            goal_door = tilesets[i].toObject()["firstgid"].toInt();
        if(tilename == "brick.tsx")
            brick = tilesets[i].toObject()["firstgid"].toInt();

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
                    int adjustheight=50;
                    if(data[j] == stage1_1){
                        Stages* stage = new Stages(":/Image/background/Stage1(1).png",
                                                   Scene, 0, y);
                        if(game_scene==scene_1)
                            stage->setPos(0, y-stage->pixmap().height()+60);
                        Scene->addItem(stage);
                    }
                    if(data[j] ==stage1_2){
                        QPixmap img(":/Image/background/Stage1(2).png");
                        QPixmap scaled_img = img.scaled(img.width()+100,img.height()+100,Qt::IgnoreAspectRatio);
                        Stages* stage = new Stages(":/Image/background/Stage1(2).png",
                                                   Scene, x, y);
                        if(game_scene==scene_1)
                            stage->setPos(x, y-stage->pixmap().height()+60);
                        stage->setPixmap(scaled_img);
                        Scene->addItem(stage);
                    }
                    if(data[j] == stage1_3){
                        Stages* stage = new Stages(":/Image/background/Stage1(3).png",
                                            Scene, x, y);
                        if(game_scene==scene_1)
                            stage->setPos(x, y -(stage->pixmap().height())+60);
                        else if(game_scene==scene_2)
                            stage->setPos(x,y-stage->pixmap().height()+adjustheight);
                        Scene->addItem(stage);
                    }
                    if(data[j] == stage2_1){
                        Stages* stage = new Stages(":/Image/background/Stage2(1).png",
                                            Scene, x, y);
                        stage->setPos(x, y -(stage->pixmap().height())+adjustheight);
                        Scene->addItem(stage);
                    }
                    if(data[j] == stage2_2){
                        Stages* stage = new Stages(":/Image/background/Stage2(2).png",
                                            Scene, x, y);
                        stage->setPos(x, y -(stage->pixmap().height())+adjustheight);
                        Scene->addItem(stage);
                    }
                    if(data[j]== floor){
                       // std::cout<<"floor";
                        Scene->addItem(new Platform(":/Image/item/floor.png",Scene,x,y));
                    }
                    if(data[j] == door){
                        Door* portal = new Door(":/Image/item/door.png",Scene);
                        portal->setPos(x,y-(portal->pixmap().height())+120);
                        Scene->addItem(portal);
                    }
                    if(data[j]==goal_door){
                        Door* portal = new Door(":/Image/item/goal_door.png",Scene);
                        portal->setPos(x,y-(portal->pixmap().height())+50);
                        Scene->addItem(portal);
                    }
                    if(data[j] == brick){
                        Stages* Brick = new Stages(":/Image/item/brick.png",Scene, x, y);
                        Brick->setPos(x,y-(Brick->pixmap().height())+adjustheight);
                        Scene->addItem(Brick);
                    }
                }
            }
        }else if(layers[i].toObject()["type"] == "objectgroup"){
            QJsonArray objects = layers[i].toObject()["objects"].toArray();
            for(int j=0;j<objects.size();j++){
                if(objects[j].toObject()["name"] == "PlayerSpawn"){

                    double x = objects[j].toObject()["x"].toDouble();
                    double y = objects[j].toObject()["y"].toDouble();
                    if(!player){
                        player = new Kirby(":/Image/Kirby_normal/kirby_stop_R.png",Scene, x, y,kirby_lives);
                        Scene ->addItem(player);
                    }
                    y=y-37;
                    player->setPos(x,y);
//std::cout<<"Player: "<<player->x()<<' '<<player->y();
                    player->y_pre_frame=static_cast<int>(y);
                }
                if(objects[j].toObject()["name"] == "EnemySpawn"){

                   double x= objects[j].toObject()["x"].toDouble();
                   double y= objects[j].toObject()["y"].toDouble();
                   Enemy* enemy =nullptr;
                   int id=objects[j].toObject()["id"].toInt();
                   if( id==12||id ==5||id ==10)
                       enemy = new Enemy(":/Image/Sparky/Sparky_attack_1.png",Scene, x, y, game_scene,true,objects[j].toObject()["id"].toInt());
                   else enemy = new Enemy(":/Image/Sparky/Sparky_attack_1.png",Scene, x, y, game_scene,false,objects[j].toObject()["id"].toInt());
                   enemys.append(enemy);
                   Scene->addItem(enemy);
                   enemy->setPos(x,y);
                }
                if(objects[j].toObject()["name"] == "MaxTomato"){
                    double x= objects[j].toObject()["x"].toDouble();
                    double y= objects[j].toObject()["y"].toDouble();
                    QGraphicsPixmapItem* tomato = nullptr;
                    tomato=new QGraphicsPixmapItem(QPixmap(":/Image/item/Maxim Tomato.png"));
                    tomato->setData(0,"MaxTomato");
                    tomato->setPos(x-50,y-40);
                    Scene->addItem(tomato);
                }
                if(objects[j].toObject()["name"] == "life"){
                    double x= objects[j].toObject()["x"].toDouble();
                    double y= objects[j].toObject()["y"].toDouble();
                    QGraphicsPixmapItem* life = nullptr;
                    life=new QGraphicsPixmapItem(QPixmap(":/Image/item/1UP.png"));
                    life->setData(0,"life");
                    life->setPos(x-50,y-35);
                    Scene->addItem(life);
                }
            }

        }
    }

}


void MainWindow::projUpdate()
{
    QList<QGraphicsItem*> allitem=Scene->items();
    for(QGraphicsItem* item : allitem)
    {
        if(item->data(0)=="Fireball"||item->data(0)=="Bullet")
        {
            Projectile* proj= dynamic_cast<Projectile*>(item);
            proj->update();
        }
    }
}

void MainWindow::kirby_status(){

    lives_Label->show();
    lives->setPixmap(QPixmap(":/Image/item/life.png"));
    lives->show();
    for(int i=0;i<player->max_hp;i++)
    {
        hearts[i]->show();

        if(i<player->hp){
            hearts[i]->setPixmap(QPixmap(":/Image/item/HP_1.png"));
        }else hearts[i]->setPixmap(QPixmap(":/Image/item/HP_0.png"));
    }

    lives_Label->clear();
    lives_Label->hide();
    if(player->life==3){
        lives_Label->setPixmap(QPixmap(":/Image/item/lives_02.png"));
    }else if(player->life==2){
        lives_Label->setPixmap(QPixmap(":/Image/item/lives_01.png"));
    }else if(player->life==4){
        lives_Label->setPixmap(QPixmap(":/Image/item/lives_03.png"));
    }else lives_Label->setPixmap(QPixmap(":/Image/item/lives_00.png"));
    lives_Label->show();

    if(player->getState()==state_spark){
        state_Label->show();
        state_Label->setPixmap(QPixmap(":/Image/Kirby_spark/Kirby_spark_board.png").scaled(state_img_width,state_img_height));
    }else if(player->getState()==state_fire){
        state_Label->show();
        state_Label->setPixmap(QPixmap(":/Image/Kirby_fire/kirbyfire_board.png").scaled(state_img_width,state_img_height));
    }else {
        state_Label->hide();
    }

}

void MainWindow::kirby_init(){

    double startX =500;
    double startY = 900;

    for (QLabel* heart : hearts) {
            if (heart) delete heart;
        }
        hearts.clear();

    for (int i = 0; i < 3; ++i) {
            QLabel* heart = new QLabel(this);
            heart->hide();
            heart->setGeometry(startX +900+ i * 60, startY, 50, 100);
            heart->setScaledContents(true);
            hearts.append(heart);
        }

        //num of lives
        if(lives_Label==nullptr)
            lives_Label = new QLabel(this);
        lives_Label->hide();
        lives_Label->setGeometry(startX+950, startY+110, 100, 50);
        lives_Label->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");

        if(lives==nullptr)
            lives = new QLabel(this);
        lives->hide();
        lives->setGeometry(startX+900, startY+110,50,50);
        lives->setScaledContents(true);

        if(state_Label==nullptr)
            state_Label = new QLabel(this);

        state_Label->hide();

        state_Label->setGeometry(50, 880, state_img_width, state_img_height);

  }
