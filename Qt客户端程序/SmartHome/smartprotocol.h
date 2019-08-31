#ifndef SMARTPROTOCOL_H
#define SMARTPROTOCOL_H
#include "userinformation.h"
#include <QJsonObject>


class SmartProtocol
{
public:
    enum{
        USER_REGISTER        = 1,
        USER_LOGIN           = 2,
        USER_DEVICE_CONTOL   = 3,
        USER_FORGET_PASSWORD = 4,
        USER_RESER_PASSWORD  = 5,
        USER_RESET_SUCCESS   = 6,
    };
    SmartProtocol();
    SmartProtocol(const QByteArray &jsonData);
    SmartProtocol(const UserInformation &info);
    SmartProtocol(const QString &userName,const QString &password);
    SmartProtocol(const QString &clientId,int deviceId,int statu);

    void addFunctionId(int id);
    QByteArray toProtocolData();
    bool getSmartProtocolStatu();
    QString getSmartProtocolError();
    int getSmartProtocolFunctionId();
    QString getSmartProtocolClientId();
    void smartProtocol(const QString &username, const QString email);
    void authProtocol(const QString &auth, const QString &password);
private:
    QJsonObject content;
};

#endif // SMARTPROTOCOL_H
