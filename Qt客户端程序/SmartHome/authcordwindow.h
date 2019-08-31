#ifndef AUTHCORDWINDOW_H
#define AUTHCORDWINDOW_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class AuthCordWindow;
}

class AuthCordWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AuthCordWindow(QWidget *parent = 0);
    ~AuthCordWindow();
signals:
    void resetSuccess(const QString &auth,const QString &password);
public slots:
    void showResetWindow(const QString &error_string);
    void on_Submit_clicked();

private:
    Ui::AuthCordWindow *ui;
    QTcpSocket *tcp;
};

#endif // AUTHCORDWINDOW_H
