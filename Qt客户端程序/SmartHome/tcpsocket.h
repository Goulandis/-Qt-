#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QTcpSocket>
#include "userinformation.h"

class TcpSocket :public QObject
{
    Q_OBJECT

public:
    TcpSocket();
    ~TcpSocket();
    void controlDeviceByTcp(int deviceId,int deviceStatu,QString clientId);
signals:
    void registerStatu(const QString &error);
    void loginStatu(const QString &error,const QString &clientId);
    void forgetPwStatu();
    void resetPassword(const QString &errorString);
public slots:
    void registerUserByTcp(const UserInformation &info);
    void recvTcpData();
    void loginUserByTcp(const QString &name,const QString &password);
    void forgetByTcp(const QString &name,const QString &email);
    void resetSuccessByTcp(const QString &auth, const QString &password);
private:
    QTcpSocket *tcp;
};

#endif // TCPSOCKET_H
