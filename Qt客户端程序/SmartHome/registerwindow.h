#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include "userinformation.h"

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = 0);
    ~RegisterWindow();
    bool checkEmail(const QString &email);
signals:
    void registerUserReady(const UserInformation &info);
public slots:
    void registerWindow();

    void on_Submit_clicked();

    void showRegisterResult(const QString &error);

private:
    Ui::RegisterWindow *ui;
};

#endif // REGISTERWINDOW_H
