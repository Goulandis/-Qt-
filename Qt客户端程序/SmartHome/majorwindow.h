#ifndef MAJORWINDOW_H
#define MAJORWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDate>
#include "roomwindow.h"
#include "tcpsocket.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class MajorWindow;
}

class MajorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MajorWindow(QWidget *parent = 0);
    ~MajorWindow();
    void setClientId(const QString &value);
    QString getClientId() const;
    void timerEvent(QTimerEvent *event);

private slots:
    void on_pushButton_3_clicked();

    void controlDeviceSlot(int deviceId,int deviceStatu);

    void on_Video_clicked();

    void on_pushButton_4_clicked();

    void replyFinished(QNetworkReply* reply);

private:
    Ui::MajorWindow *ui;
    QString clientId;
    RoomWindow *room;
    TcpSocket *tcp;
    QNetworkAccessManager *manager;
};

#endif // MAJORWINDOW_H
