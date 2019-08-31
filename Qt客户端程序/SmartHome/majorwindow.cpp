#include "majorwindow.h"
#include "ui_majorwindow.h"
#include "widget.h"
#include "mediawindow.h"
#include <QTime>

MajorWindow::MajorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MajorWindow)
{
    ui->setupUi(this);
    this->startTimer(1000);
    room = new RoomWindow;
    connect(room,SIGNAL(controlDevice(int,int)),this,SLOT(controlDeviceSlot(int,int)));
    tcp = new TcpSocket;
    manager = new QNetworkAccessManager;
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
}

MajorWindow::~MajorWindow()
{
    delete ui;
}

void MajorWindow::setClientId(const QString &value)
{
    clientId = value;
}

QString MajorWindow::getClientId() const
{
   return clientId;
}

void MajorWindow::timerEvent(QTimerEvent *event)
{
    QTime time = QTime::currentTime();
    ui->TimeLabel->setText(time.toString("hh:mm:ss"));
    QDate date = QDate::currentDate();
    ui->DateLabel->setText(date.toString("yyyy-MM-dd dddd"));

    QNetworkRequest request;
    request.setUrl(QUrl("https://www.tianqiapi.com/api/?version=v6&cityid=101200901"));
    manager->get(request);

}

void MajorWindow::on_pushButton_3_clicked()
{
    room->show();
}

void MajorWindow::controlDeviceSlot(int deviceId, int deviceStatu)
{
    tcp->controlDeviceByTcp(deviceId,deviceStatu,clientId);
}

void MajorWindow::on_Video_clicked()
{
    Widget *video = new Widget;
    video->show();
}

void MajorWindow::replyFinished(QNetworkReply *reply)
{
    QString recv = reply->readAll();
   // qDebug()<<recv;
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(recv.toUtf8(),&jsonError);
    if(jsonDoc.isEmpty() || jsonError.error != QJsonParseError::NoError){
        qDebug()<<"fail to from json"<<jsonError.errorString();
        return ;
    }
    QJsonObject content = jsonDoc.object();
    QString temp = content["tem"].toString();
    QString humi = content["humidity"].toString();
    QString weather = content["wea"].toString();
    QString _temp = QString("%1℃").arg(temp);
    ui->temp->setText(_temp);
    ui->humi->setText(humi);
    ui->weather->setText(weather);
    //qDebug()<<temp<<"  "<<humi<<"  "<<weather;
    reply->deleteLater();
}

void MajorWindow::on_pushButton_4_clicked()
{
    mediaWindow *media = new mediaWindow;
    media->show();
}
