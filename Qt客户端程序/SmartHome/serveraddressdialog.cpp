#include "serveraddressdialog.h"
#include "ui_serveraddressdialog.h"
#include <QHostAddress>
#include <QFile>

using namespace std;

QString ServerAddressDialog::serverIP;
short ServerAddressDialog::serverPort;

ServerAddressDialog::ServerAddressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerAddressDialog)
{
    ui->setupUi(this);
}

ServerAddressDialog::~ServerAddressDialog()
{
    delete ui;
}

QString ServerAddressDialog::getServerIP()
{
    return serverIP;
}

void ServerAddressDialog::setServerIP(const QString &value)
{
    serverIP = value;
}

short ServerAddressDialog::getServerPort()
{
    return serverPort;
}

void ServerAddressDialog::setServerPort(short value)
{
    serverPort = value;
}

void ServerAddressDialog::on_Submit_clicked()
{
    bool ipOk,portOk;
    QString ip = ui->IP->text();
    QHostAddress ipAddress(ip);
    if(ip.isEmpty() || ipAddress.isNull()){
        ui->IPTag->setText("请输入正确的服务器地址");
        ipOk = false;
    }
    else{
        ui->IPTag->setText("");
        ipOk = true;
    }
    QString port = ui->Port->text();
    bool ok;
    port.toShort(&ok);
    if(port.isEmpty() || !ok){
        ui->PortTag->setText("请输入正确的服务器端口");
        portOk = false;
    }
    else{
        ui->PortTag->setText("");
        portOk = true;
    }
    if(ipOk && portOk){
        serverIP = ip;
        serverPort = port.toShort(&ok);
        QFile file("B:/SCM/Qt/QtProject/QtProject/SmartHome/image/IPInfo.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(ip.toUtf8());
        file.write("\n");
        file.write(port.toUtf8());
        file.write("\n");
        this->close();
    }
}

void ServerAddressDialog::rememberIP()
{
    QFile ipFile("B:/SCM/Qt/QtProject/QtProject/SmartHome/image/IPInfo.txt");
    ipFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString ip = ipFile.readLine();
    ip.remove(ip.length()-1,1);
    QString port = ipFile.readLine();
    port.remove(port.length()-1,1);
    ipFile.close();
    ui->IP->setText(ip);
    ui->Port->setText(port);
}
