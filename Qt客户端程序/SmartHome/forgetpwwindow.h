#ifndef FORGETPWWINDOW_H
#define FORGETPWWINDOW_H

#include <QMainWindow>

namespace Ui {
class ForgetPwWindow;
}

class ForgetPwWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ForgetPwWindow(QWidget *parent = 0);
    ~ForgetPwWindow();
signals:
    void forget_infoToTcp(const QString &name,const QString &email);


private slots:
    void on_Submit_clicked();
    void showAuthWindow();

private:
    Ui::ForgetPwWindow *ui;
};

#endif // FORGETPWWINDOW_H
