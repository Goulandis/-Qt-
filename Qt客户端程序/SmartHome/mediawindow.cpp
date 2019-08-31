#include "mediawindow.h"
#include "ui_mediawindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QScrollBar>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
#include <QDebug>
#include <QListWidget>

mediaWindow::mediaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mediaWindow)
{
    ui->setupUi(this);
    manager1 = new QNetworkAccessManager;
    manager2 = new QNetworkAccessManager;
    connect (manager1,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    connect(manager2,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished2(QNetworkReply*)));
    player = new QMediaPlayer;
    playlist = new QMediaPlaylist;
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged(qint64)));
    player->setVolume(50);
    ui->slider->setMinimum(0);
    ui->slider->setMaximum(1000);
    ui->slider->setSingleStep(500);
    ui->slider->setValue(500);
    player->setPlaylist(playlist);
    connect(playlist,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(setFocusOn()));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(getPosition(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(getDuration(qint64)));
}

mediaWindow::~mediaWindow()
{

    delete ui;
    delete manager1;
    delete manager2;
    delete player;
    delete playlist;
}

void mediaWindow::addItem(QString playname)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(playname);
    ui->listWidget->addItem(item);
}

void mediaWindow::createKeyValue(const QString playname, const QString playurl, const QString lyric)
{
    key_value[playname] = playurl;
    key_value[playurl] = lyric;
}

void mediaWindow::playSong(QListWidgetItem *item)
{
    ui->listWidget->setFocus();
    ui->listWidget->setCurrentItem(item);
    QString playname = item->text();
    QString playurl = key_value[playname].toString();
    QString lyric = key_value[playurl].toString();
    ui->lyricEdit->setText(lyric);
}

void mediaWindow::allLoopPlay(int position)
{
    playlist->setCurrentIndex(position);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->play();
    QListWidgetItem *item = ui->listWidget->item(position);
    playSong(item);
}

void mediaWindow::signalLoopPlay(int position)
{
      playlist->setCurrentIndex(position);
      playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
      player->play();
      QListWidgetItem *item = ui->listWidget->item(position);
      playSong(item);

}

void mediaWindow::randomPlay(int position)
{
       playlist->setCurrentIndex(position);
       playlist->setPlaybackMode(QMediaPlaylist::Random);
       player->play();
       QListWidgetItem *item = ui->listWidget->item(position);
       playSong(item);
}

void mediaWindow::signalPlay(int position)
{
    playlist->setCurrentIndex(position);
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    player->play();
    //qDebug()<<"playList"<<playlist->currentIndex();
    QListWidgetItem *item = ui->listWidget->item(position);
    playSong(item);
}


void mediaWindow::on_searchPushButton_clicked()
{
    QString str = ui->songlineEdit->text();
    QString url = QString("https://songsearch.kugou.com/song_search_v2?keyword=%1&page=1&pagesize=30&userid=-1&clientver=&platform=WebFilter&tag=em&filter=2&iscorrection=1&privilege_filter=0&_=1562688005410").arg(str);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    manager1->get(request);
}

void mediaWindow::replyFinished(QNetworkReply *reply)
{
    QString recv = reply->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(recv.toUtf8(),&jsonError);
    if(jsonDoc.isEmpty() || jsonError.error != QJsonParseError::NoError){
        qDebug()<<"fail to from json"<<jsonError.errorString();
        return ;
    }

    QString fileHash;
    QJsonObject rootObj = jsonDoc.object();
    QString albumId;
    QString _time;
    if(rootObj.contains("data")){
        QJsonObject valueObj = (rootObj.value("data")).toObject();
        if(valueObj.contains("lists")){
            QJsonArray array= (valueObj.value("lists")).toArray();
            int size = array.size();
            for(int i=0;i<size;i++){
                QJsonObject content = (array.at(i)).toObject();
                if(content.contains("AlbumID")){
                   albumId = content["AlbumID"].toString();
                   qDebug()<<albumId;
                }
                if(content.contains("FileHash")){
                    fileHash = content["FileHash"].toString();
                    qDebug()<<fileHash;
                    //D398A3EFFC9891C8178843F5476BDF35
                }
                QDateTime time;
                time = QDateTime::currentDateTime();
                time.setTimeSpec(Qt::UTC);
                long long seconds2 = time.toMSecsSinceEpoch();
                _time = QString::number(seconds2,10);
               //c49f7727b4c63a710d608ae1a63dc6b8谷歌浏览器 47d07e00432fbf8600086e4351a56c71 360极速浏览器
               QString url = QString("https://wwwapi.kugou.com/yy/index.php?r=play/getdata&hash=%1&album_id=%2&mid=dbb390f565acd06a9c156900d3d6b24a").arg(fileHash).arg(albumId);

               qDebug()<<url;

               QNetworkRequest request;
               request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded;charset=UTF-8");
               request.setUrl(QUrl(url));
               manager2->get(request);
   //            return ;
            }
        }
    }
    else{
        qDebug()<<"search failed";
    }

}

void mediaWindow::replyFinished2(QNetworkReply *reply)
{
    QString recv = reply->readAll();
    qDebug()<<recv;
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(recv.toUtf8(),&jsonError);
    if(jsonDoc.isEmpty() || jsonError.error != QJsonParseError::NoError){
        qDebug()<<"fail to from json"<<jsonError.errorString();
        return ;
    }

    QString songUrl;
    QString lyrics;
    QString playname;
    QJsonObject rootObj = jsonDoc.object();
    if(rootObj.contains("data")){
        QJsonObject content = (rootObj.value("data")).toObject();
        lyrics = content["lyrics"].toString();
        songUrl = content["play_url"].toString();
        playname = content["audio_name"].toString();
        createKeyValue(playname,songUrl,lyrics);
        addItem(playname);
        playlist->addMedia(QUrl(songUrl));
    }
}



void mediaWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int position = ui->listWidget->currentRow();
    qDebug()<<position;
    int playModel = ui->comboBox->currentIndex();
    switch(playModel){
    case 0:
        signalPlay(position);
        break;
    case 1:
        signalLoopPlay(position);
        break;
    case 2:
        allLoopPlay(position);
        break;
    case 3:
        randomPlay(position);
        break;
    }
    return;
}

void mediaWindow::on_pushButton_clicked()
{
    int num = ui->listWidget->count();
    ui->listWidget->setFocus();
    for(int i=0;i<num;i++){
        QListWidgetItem *item = ui->listWidget->takeItem(ui->listWidget->currentRow());
        QString str = item->text();
        key_value.remove(str);
        delete item;
    }
    playlist->removeMedia(0,num-1);
    ui->lyricEdit->clear();
    ui->songlineEdit->clear();
}

void mediaWindow::on_pausePushButton_clicked(bool checked)
{
    if(checked == true){
        player->pause();
        ui->pausePushButton->setText("播放");
    }
    else{
        player->play();
        ui->pausePushButton->setText("暂停");
    }
}

void mediaWindow::on_slider_sliderMoved(int position)
{
    int value = ui->slider->value();
    player->setVolume(value);
}

void mediaWindow::on_lastPushButton_clicked()
{
    int position  = ui->listWidget->currentRow();
    if(position == 0){
        int num = ui->listWidget->count();
        position = num-1;
    }
    else{
        position-=1;
    }
    signalPlay(position);
}

void mediaWindow::on_nextPushButton_clicked()
{
    int position  = ui->listWidget->currentRow();
    if(position == ui->listWidget->count()-1){
        position = 0;
    }
    else{
        position+=1;
    }
    signalPlay(position);
}

void mediaWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->listWidget->setFocus();
    int position = ui->listWidget->currentRow();
    switch(index){
    case 0:
        signalPlay(position);
        break;
    case 1:
        signalLoopPlay(position);
        break;
    case 2:
        allLoopPlay(position);
        break;
    case 3:
        randomPlay(position);
        break;
    }
    return;
}

void mediaWindow::setFocusOn()
{
    if(ui->comboBox->currentIndex() == 0)
        return;
    int position = playlist->currentIndex();
    qDebug()<<"playList current: "<<position;
    QListWidgetItem *item = ui->listWidget->item(position);
    playSong(item);
}

void mediaWindow::on_playProcessSlider_sliderMoved(int position)
{
    qint64 value = ui->playProcessSlider->value();
    player->setPosition(value);
}

void mediaWindow::getDuration(qint64 duration)
{
    duration = player->duration();
    ui->playProcessSlider->setMinimum(0);
    ui->playProcessSlider->setMaximum(duration);
    ui->playProcessSlider->setSingleStep(10000);
    int m,s;
    duration /= 1000;
    m = duration/60;
    s = duration-60*m;
    QString str;
    ui->durationLabel->setText(QString("%1:%2").arg(m).arg(s));
}

void mediaWindow::getPosition(qint64 position)
{
    position = player->position();
    int m,s;
    position /= 1000;
    m = position/60;
    s = position-60*m;
    QString str;
    ui->positionLabel->setText(QString("%1:%2").arg(m).arg(s));
}

void mediaWindow::on_listWidget_clicked(const QModelIndex &index)
{

}
