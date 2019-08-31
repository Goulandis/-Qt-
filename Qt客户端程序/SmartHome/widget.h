#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QFile>
#include <QImage>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    enum process{
      START,
      NEXT1,
      NEXT2,
      END,
    };

private slots:
    void on_pushButton_clicked();
    void readHttpCameraData();
    void finishedHttp();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QFile file;
    QImage image;
    qint32 currentLen;
    qint32 totalLen;
    QByteArray buf;
    QByteArray boundary;
    enum process statu;
    Ui::Widget *ui;
    QNetworkAccessManager *netwokManger;
    QNetworkReply *networkReply;
};

#endif // WIDGET_H
