#include "smartprotocol.h"
#include <QJsonDocument>
#include <QDebug>

SmartProtocol::SmartProtocol()
{

}

SmartProtocol::SmartProtocol(const QByteArray &jsonData)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData,&jsonError);
    if(jsonDoc.isEmpty() || jsonError.error != QJsonParseError::NoError){
        qDebug()<<"fail to from json"<<jsonError.errorString();
        return;
    }
    content = jsonDoc.object();
}

SmartProtocol::SmartProtocol(const UserInformation &info)
{
    content.insert("username",info.getName());
    content.insert("password",info.getPassword());
    content.insert("email",info.getEmail());
}

SmartProtocol::SmartProtocol(const QString &userName, const QString &password)
{
    content.insert("username",userName);
    content.insert("password",password);
}

SmartProtocol::SmartProtocol(const QString &clientId, int deviceId, int statu)
{
    content["clientId"] = clientId;
    content["deviceId"] = deviceId;
    content["controlStatu"] = statu;
}

void SmartProtocol::smartProtocol(const QString &username, const QString email)
{
    content.insert("username",username);
    content.insert("email",email);
}

void SmartProtocol::authProtocol(const QString &auth,const QString &password){
     content.insert("authcord",auth);
      content.insert("password",password);
}

void SmartProtocol::addFunctionId(int id)
{
    content["functionId"] = id;
}

QByteArray SmartProtocol::toProtocolData()
{
    QJsonDocument jsonDoc(content);
    return jsonDoc.toJson();
}

bool SmartProtocol::getSmartProtocolStatu()
{
    int ret = content["statu"].toInt();
    qDebug()<<ret;
    if(ret != 0){
        return false;
    }
    return true;
}

QString SmartProtocol::getSmartProtocolError()
{
    QString errorString = content["error"].toString();
    return errorString;
}

int SmartProtocol::getSmartProtocolFunctionId()
{
    return content["functionId"].toInt();
}

QString SmartProtocol::getSmartProtocolClientId()
{
    return content["clientId"].toString();
}
