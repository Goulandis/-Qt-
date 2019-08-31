#include "registerwindow.h"
#include "ui_registerwindow.h"
#include <QMessageBox>
#include "tcpsocket.h"

RegisterWindow::RegisterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);

    TcpSocket *tcpSocket = new TcpSocket;
    connect(this,SIGNAL(registerUserReady(UserInformation)),tcpSocket,SLOT(registerUserByTcp(UserInformation)));
    connect(tcpSocket,SIGNAL(registerStatu(QString)),this,SLOT(showRegisterResult(QString)));
}

RegisterWindow::~RegisterWindow()
{
    delete ui;

}

bool RegisterWindow::checkEmail(const QString &email)
{
    bool ok = email.contains("@") && email.endsWith(".com");
    if(!ok) return false;
    QString str1 = email.section('@',0,0);//
    if(str1.isEmpty()) return false;
    QString str2 = email.section('@',1,1);
    if(str2.isEmpty()) return false;

    return true;
}

void RegisterWindow::registerWindow()
{
    ui->Name->clear();
    ui->Email->clear();
    ui->Password->clear();
    ui->Echo->clear();
    this->show();
}

void RegisterWindow::on_Submit_clicked()
{
    bool nameOk,emailOk,passwordOk,echoOk;
    QString name = ui->Name->text();
    if(name.isEmpty()){
        ui->NameTag->setText("用户名不能为空");
        nameOk = false;
    }
    else{
        ui->NameTag->setText("");
        nameOk = true;
    }
    QString email = ui->Email->text();
    if(email.isEmpty()){
        ui->EmailTag->setText("邮箱不能为空");
        emailOk = false;
    }
    if(checkEmail(email) == false){
        ui->EmailTag->setText("请填写正确的邮箱");
        emailOk = false;
    }
    else{
        ui->EmailTag->setText("");
        emailOk = true;
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
    QString echo = ui->Echo->text();
    if(echo != password){
        ui->EchoTag->setText("两次密码不一致");
        echoOk = false;
    }
    else{
        ui->EchoTag->setText("");
        echoOk = true;
    }
    if(nameOk && emailOk && passwordOk && echoOk){
        UserInformation userInfo(name,email,password);
        emit registerUserReady(userInfo);
        //QMessageBox::warning(NULL,"注册信息","注册成功",QMessageBox::Yes,QMessageBox::Yes);
        this->close();
    }
}

void RegisterWindow::showRegisterResult(const QString &error)
{
    if(error.isEmpty()){
        QMessageBox::information(NULL,"注册用户","注册成功",QMessageBox::Yes,QMessageBox::Yes);
    }
    else{
        QString str = QString("注册失败:%1").arg(error);
        QMessageBox::warning(NULL,"注册用户",str,QMessageBox::Yes,QMessageBox::Yes);
    }
}

