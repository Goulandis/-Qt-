#include "authcordwindow.h"
#include "ui_authcordwindow.h"
#include "tcpsocket.h"
#include "resetpwwindow.h"
#include <QMessageBox>
#include "smartprotocol.h"
#include "tcpsocket.h"
#include "serveraddressdialog.h"
#include <QHostAddress>
#include <QDebug>

AuthCordWindow::AuthCordWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthCordWindow)
{
    ui->setupUi(this);

    tcp = new QTcpSocket;
    TcpSocket *resetTcp = new TcpSocket;
    connect(resetTcp,SIGNAL(resetPassword(QString)),this,SLOT(showResetWindow(QString)));
    connect(this,SIGNAL(resetSuccess(QString,QString)),resetTcp,SLOT(resetSuccessByTcp(QString,QString)));

}

AuthCordWindow::~AuthCordWindow()
{
    delete ui;

}

void AuthCordWindow::showResetWindow(const QString &error)
{
    qDebug()<<error;
    if(error.isEmpty()){
        QMessageBox::warning(NULL,"验证码校验","密码重置成功",QMessageBox::Yes,QMessageBox::Yes);
        this->close();
    }
    else
    {
        QMessageBox::warning(NULL,"验证码校验",error,QMessageBox::Yes,QMessageBox::Yes);
     }
}

void AuthCordWindow::on_Submit_clicked()
{


    QString auth = ui->AuthCord->text();
    QString password = ui->Password->text();
    emit resetSuccess(auth,password);
}
