#include "mainwindow.h"

void MainWindow::loadTiledMap(QString json_path){
    QFile file(json_path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Fail to find json file";
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject mapdata = doc.object();

    int mapWidth = mapdata["width"].toInt();
    int tileWidth = mapdata["tilewidth"].toInt();
    int tileHeight = mapdata["tileHeight"].toInt();

    QJsonArray layers = mapdata["layers"].toArray();
    for(int i=0;i<layers.size();i++){
        if(layers[i].toObject()["type"] == "tilelayer"){
            QJsonArray data = layers[i].toObject()["data"].toArray();
            for(int j=0;j<data.size();j++){
                if(data[i]==0)
                    continue;

                int x = (j % mapWidth) * tileWidth;
                int y = (j / mapWidth)* tileHeight;
                Scene->addItem(new Platform(":/Image/item/floor.png",Scene,x,y));

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
