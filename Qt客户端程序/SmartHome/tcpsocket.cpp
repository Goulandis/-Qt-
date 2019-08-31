#include "tcpsocket.h"
#include <QDebug>
#include "serveraddressdialog.h"
#include <QMessageBox>
#include <QHostAddress>
#include "smartprotocol.h"
#include <QDebug>
#include "userinformation.h"

TcpSocket::TcpSocket()
{
    tcp = new QTcpSocket;
    connect(tcp,SIGNAL(readyRead()),this,SLOT(recvTcpData()));
}

TcpSocket::~TcpSocket()
{
    delete tcp;
}

void TcpSocket::controlDeviceByTcp(int deviceId, int deviceStatu, QString clientId)
{
    QString ip = ServerAddressDialog::getServerIP();
    short port = ServerAddressDialog::getServerPort();

    if(ip.isEmpty() || port == 0){
        QMessageBox::warning(NULL,"没有设置服务器地址信息","服务器连接错误",QMessageBox::Yes,QMessageBox::Yes);
        return;
    }
    if(tcp->state() == QTcpSocket::UnconnectedState){
        tcp->connectToHost(QHostAddress(ip),port);
    }
    SmartProtocol smartProtocol(clientId,deviceId,deviceStatu);
    smartProtocol.addFunctionId(SmartProtocol::USER_DEVICE_CONTOL);
    tcp->write(smartProtocol.toProtocolData());

    return;
}

void TcpSocket::recvTcpData()
{
    QString errorString;
    QByteArray tcpData = tcp->readAll();
    SmartProtocol smartProtocol(tcpData);
    if(smartProtocol.getSmartProtocolStatu()){
        errorString = "";
    }
    else{
        errorString = smartProtocol.getSmartProtocolError();
    }

    switch(smartProtocol.getSmartProtocolFunctionId()){

    case SmartProtocol::USER_REGISTER:
        emit registerStatu(errorString);
        break;
    case SmartProtocol::USER_LOGIN:
        qDebug()<<"--1";
        emit loginStatu(errorString,smartProtocol.getSmartProtocolClientId());
        break;
    case SmartProtocol::USER_FORGET_PASSWORD:
        emit forgetPwStatu();
        break;
    case SmartProtocol::USER_RESET_SUCCESS:
        emit resetPassword(errorString);
        break;
    }

    return;
}

void TcpSocket::registerUserByTcp(const UserInformation &info)
{
    QString ip = ServerAddressDialog::getServerIP();
    short port = ServerAddressDialog::getServerPort();
    if(ip.isEmpty() || port == 0){
        QMessageBox::warning(NULL,"提示","请输入服务器地址信息后再注册",QMessageBox::Yes, QMessageBox::Yes);
    }
    if(tcp->state() == QTcpSocket::UnconnectedState){//如果没有建立连接则建立连接
        tcp->connectToHost(QHostAddress(ip),port);
    }
    SmartProtocol smartProtocol(info);
    smartProtocol.addFunctionId(SmartProtocol::USER_REGISTER);
    tcp->write(smartProtocol.toProtocolData());
    return;
}

void TcpSocket::loginUserByTcp(const QString &name, const QString &password)
{
    QString ip = ServerAddressDialog::getServerIP();
    short port = ServerAddressDialog::getServerPort();
    if(ip.isEmpty() || port == 0){
        QMessageBox::warning(NULL,"提示","请输入服务器地址信息后再登陆",QMessageBox::Yes, QMessageBox::Yes);
    }
    if(tcp->state() == QTcpSocket::UnconnectedState){//如果没有建立连接则建立连接
        tcp->connectToHost(QHostAddress(ip),port);
    }
    SmartProtocol smartProtocol(name,password);
    smartProtocol.addFunctionId(SmartProtocol::USER_LOGIN);
    tcp->write(smartProtocol.toProtocolData());
    return;
}

void TcpSocket::forgetByTcp(const QString &name, const QString &email)
{
    QString ip = ServerAddressDialog::getServerIP();
    short port = ServerAddressDialog::getServerPort();
    if(ip.isEmpty() || port == 0){
        QMessageBox::warning(NULL,"提示","请输入服务器地址信息后再登陆",QMessageBox::Yes, QMessageBox::Yes);
    }
    if(tcp->state() == QTcpSocket::UnconnectedState){//如果没有建立连接则建立连接
        tcp->connectToHost(QHostAddress(ip),port);
    }

    SmartProtocol smartProtocol;
    smartProtocol.smartProtocol(name,email);
    smartProtocol.addFunctionId(SmartProtocol::USER_FORGET_PASSWORD);
    tcp->write(smartProtocol.toProtocolData());

    return;
}

void TcpSocket::resetSuccessByTcp(const QString &auth,const QString &password)
{
    SmartProtocol authProtocol;
    QString ip = ServerAddressDialog::getServerIP();
    short port = ServerAddressDialog::getServerPort();
    UserInformation info;

    authProtocol.authProtocol(auth,password);
    authProtocol.smartProtocol(info.getName(),info.getEmail());
    authProtocol.addFunctionId(SmartProtocol::USER_RESET_SUCCESS);
    if(tcp->state() == QTcpSocket::UnconnectedState){
        tcp->connectToHost(QHostAddress(ip),port);
    }
    tcp->write(authProtocol.toProtocolData());
}
