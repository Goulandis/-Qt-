#include "widget.h"
#include "ui_widget.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QPainter>
#include <QMediaPlayer>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    netwokManger = new QNetworkAccessManager(this);
    QMediaPlayer *mediaPlayer = new QMediaPlayer;
    mediaPlayer->setVolume(50);
    mediaPlayer->setVideoOutput(ui->widget);
    mediaPlayer->setMedia(QUrl("rtsp://192.168.1.3:8554/abc.mkv"));
    mediaPlayer->play();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    this->statu = START;
    this->currentLen = 0;
    QString httpUrl = "http://192.168.1.1:8080/?action=stream";
    networkReply = netwokManger->get(QNetworkRequest(QUrl(httpUrl)));
    connect(networkReply,SIGNAL(readyRead()),this,SLOT(readHttpCameraData()));
    connect(networkReply,SIGNAL(finished()),this,SLOT(finishedHttp()));
}

void Widget::readHttpCameraData()
{
     qint32 len;
     QByteArray contentTypeLine;
     QByteArray contentLenghtLine;
     QByteArray blankSpaceLine;
     QByteArray endBoundaryLine;

    switch(statu){
      case START:
           boundary = networkReply->readLine();
           statu = NEXT1;
      break;

      case NEXT1:
           contentTypeLine   = networkReply->readLine();
           contentLenghtLine = networkReply->readLine();
           blankSpaceLine    = networkReply->readLine();
           sscanf(contentLenghtLine.data(),"Content-Length: %d\r\n",&totalLen);
           buf.resize(totalLen);
           statu = NEXT2;
           break;

     case NEXT2:
        len = networkReply->read(buf.data()+ currentLen,totalLen - currentLen);
        currentLen = currentLen + len;
        if(currentLen == totalLen){
            image.loadFromData(buf);
            statu = END;
            update();
            break;
        }
        break;

    case END:
        blankSpaceLine = networkReply->readLine();
        endBoundaryLine = networkReply->readLine();
        statu = NEXT1;
        currentLen = 0;
        break;
    }
}

void Widget::finishedHttp()
{

}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.drawPixmap(this->rect().adjusted(50,50,-100,-100),QPixmap::fromImage(image));
}
