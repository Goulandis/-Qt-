#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDebug>
#include "registerwindow.h"
#include <QMessageBox>
#include "majorwindow.h"
#include <QFile>
#include "delay.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
   ui->setupUi(this);
   regWindow = new RegisterWindow;
   tcpSocket = new TcpSocket;
   serverDialog = new ServerAddressDialog;
   forgetPw = new ForgetPwWindow;
   connect(this,SIGNAL(showRegistWindow()),regWindow,SLOT(registerWindow()));
   connect(this,SIGNAL(loginUser(QString,QString)),tcpSocket,SLOT(loginUserByTcp(QString,QString)));
   connect(tcpSocket,SIGNAL(loginStatu(QString,QString)),this,SLOT(showLoginResult(QString,QString)));
   connect(this,SIGNAL(rememberIPReady()),serverDialog,SLOT(rememberIP()));
   writeAccount();
   emit rememberIPReady();
   ui->ForgetPw->setFlat(true);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::writeAccount()
{
    QFile file("B:/SCM/Qt/QtProject/QtProject/SmartHome/image/UserInfo.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString isRemember = file.readLine();
    isRemember.remove(isRemember.length()-1,1);
    QString name = file.readLine();
    name.remove(name.length()-1,1);
    QString password = file.readLine();
    password.remove(password.length()-1,1);
    file.close();
    if(isRemember == "true"){
        ui->Account->setText(name);
        ui->Password->setText(password);
        ui->RemPwTag->setText(" √");
    }
}
void LoginWindow::on_RememberPw_clicked()
{
    if(isRemember == false){
        ui->RemPwTag->setText(" √");
        isRemember = true;
        rememberString = "true";
    }
    else{
        ui->RemPwTag->setText("");
        isRemember = false;
        rememberString = "false";
    }
}

void LoginWindow::on_registerAction_triggered()
{
    emit showRegistWindow();
}

void LoginWindow::on_serverAction_triggered()
{
    emit serverDialog->show();
}

void LoginWindow::on_LoginButton_clicked()
{
    bool nameOk,passwordOk;
    QString userName = ui->Account->text();

    if(userName.isEmpty()){
        ui->AccountTag->setText("用户名不能为空");
        nameOk = false;
    }
    else{
        ui->AccountTag->setText("");
        nameOk = true;
    }
    QString password = ui->Password->text();
    if(password.isEmpty()){
        ui->PasswordTag->setText("密码不能为空");
        passwordOk = false;
    }
    else{
        ui->PasswordTag->setText("");
        passwordOk = true;
    }
    if(nameOk && passwordOk){
        QString huanH = "\n";
        emit loginUser(userName,password);
        QFile file("B:/SCM/Qt/QtProject/QtProject/SmartHome/image/UserInfo.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(rememberString.toUtf8());
        file.write(huanH.toUtf8());
        file.write(userName.toUtf8());
        file.write(huanH.toUtf8());
        file.write(password.toUtf8());
        file.write(huanH.toUtf8());
        file.close();
    }
}

void LoginWindow::showLoginResult(const QString &error,const QString &clientId)
{
    if(!error.isEmpty()){
        QString str = QString("登陆失败:%1").arg(error);
        QMessageBox::warning(NULL,"用户登陆",str,QMessageBox::Yes,QMessageBox::Yes);
        return;
    }
    MajorWindow *majorWindow = new MajorWindow;
    majorWindow->setClientId(clientId);
    majorWindow->show();
    this->close();
}

void LoginWindow::on_ForgetPw_clicked()
{
    forgetPw->show();
}
