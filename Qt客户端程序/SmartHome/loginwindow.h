#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "registerwindow.h"
#include "serveraddressdialog.h"
#include "tcpsocket.h"
#include "forgetpwwindow.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    void writeAccount();
signals:

    void showRegistWindow();

    void loginUser(const QString &name,const QString &password);

    void rememberIPReady();
private slots:

    void on_RememberPw_clicked();

    void on_registerAction_triggered();

    void on_serverAction_triggered();

    void on_LoginButton_clicked();

    void showLoginResult(const QString &error, const QString &clientId);

    void on_ForgetPw_clicked();

private:
    Ui::LoginWindow *ui;
    bool isRemember = false;
    QString rememberString;
    RegisterWindow *regWindow;
    ServerAddressDialog *serverDialog;
    TcpSocket *tcpSocket;
    ForgetPwWindow *forgetPw;
};

#endif // LOGINWINDOW_H
