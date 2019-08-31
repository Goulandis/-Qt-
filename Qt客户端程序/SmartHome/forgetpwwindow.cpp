#include "forgetpwwindow.h"
#include "ui_forgetpwwindow.h"
#include "smartprotocol.h"
#include "tcpsocket.h"
#include "authcordwindow.h"
#include "userinformation.h"


ForgetPwWindow::ForgetPwWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ForgetPwWindow)
{
    ui->setupUi(this);
    TcpSocket *forget_Tcp = new TcpSocket;

    connect(this,SIGNAL(forget_infoToTcp(QString,QString)),forget_Tcp,SLOT(forgetByTcp(QString,QString)));
    connect(forget_Tcp,SIGNAL(forgetPwStatu()),this,SLOT(showAuthWindow()));
}

ForgetPwWindow::~ForgetPwWindow()
{
    delete ui;
}

void ForgetPwWindow::on_Submit_clicked()
{
    UserInformation info;
    info.setName(ui->UserName->text());
    info.setEmail(ui->Email->text());

    emit forget_infoToTcp(info.getName(),info.getEmail());
}

void ForgetPwWindow::showAuthWindow()
{
    AuthCordWindow *authWindow = new AuthCordWindow;
    authWindow->show();
    this->close();
}
