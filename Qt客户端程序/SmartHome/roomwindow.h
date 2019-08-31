#ifndef ROOMWINDOW_H
#define ROOMWINDOW_H

#include <QMainWindow>

namespace Ui {
class RoomWindow;
}

class RoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum{
        LED_1 = 1,
        LED_2 = 2,
        FAN = 3,
        DOOR = 4,
    };
    explicit RoomWindow(QWidget *parent = 0);
    ~RoomWindow();
signals:
    void controlDevice(int deviceId,int deviceStatu);
private slots:
    void on_Led_1_clicked(bool checked);

    void on_Led_2_clicked(bool checked);

    void on_Fan_clicked(bool checked);

    void on_Door_clicked(bool checked);

private:
    Ui::RoomWindow *ui;
};

#endif // ROOMWINDOW_H
