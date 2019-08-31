#include "roomwindow.h"
#include "ui_roomwindow.h"
#include <QDebug>

RoomWindow::RoomWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoomWindow)
{
    ui->setupUi(this);
}

RoomWindow::~RoomWindow()
{
    delete ui;
}

void RoomWindow::on_Led_1_clicked(bool checked)
{
    int statu;
    if(checked){
        ui->Led_1->setStyleSheet("border-image: url(:/image/room/ON.png);");
        ui->Ledw_1->setStyleSheet("border-image: url(:/image/room/openled_ico.png);");
        statu = 0;
    }
    else{
        ui->Led_1->setStyleSheet("border-image: url(:/image/room/OFF.png);");
        ui->Ledw_1->setStyleSheet("border-image: url(:/image/room/closeled_ico.png);");
        statu = 1;
    }

    emit controlDevice(LED_1,statu);
}

void RoomWindow::on_Led_2_clicked(bool checked)
{
    int statu;
    if(checked){
        ui->Led_2->setStyleSheet("border-image: url(:/image/room/ON.png);");
        ui->Ledw_2->setStyleSheet("border-image: url(:/image/room/openled_ico.png);");
        statu = 0;
    }
    else{
        ui->Led_2->setStyleSheet("border-image: url(:/image/room/OFF.png);");
        ui->Ledw_2->setStyleSheet("border-image: url(:/image/room/closeled_ico.png);");
        statu = 1;
    }

    emit controlDevice(LED_2,statu);
}

void RoomWindow::on_Fan_clicked(bool checked)
{
    int statu;
    if(checked){
        ui->Fan->setStyleSheet("border-image: url(:/image/room/ON.png);");
        ui->Fanw->setStyleSheet("border-image: url(:/image/room/openfan_ico.png);");
        statu = 0;
    }
    else{
        ui->Fan->setStyleSheet("border-image: url(:/image/room/OFF.png);");
        ui->Fanw->setStyleSheet("border-image: url(:/image/room/closefan_ico.png);");
        statu = 1;
    }

    emit controlDevice(FAN,statu);
}

void RoomWindow::on_Door_clicked(bool checked)
{
    int statu;
    if(checked){
        ui->Door->setStyleSheet("border-image: url(:/image/room/ON.png);");
        ui->Doorw->setStyleSheet("border-image: url(:/image/room/opendoor_ico.png);");
        statu = 0;
    }
    else{
        ui->Door->setStyleSheet("border-image: url(:/image/room/OFF.png);");
        ui->Doorw->setStyleSheet("border-image: url(:/image/room/closedoor_ico.png);");
        statu = 1;
    }

    emit controlDevice(DOOR,statu);
}
