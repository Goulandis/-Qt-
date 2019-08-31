include "resetpwwindow.h"
#include "ui_resetpwwindow.h"

resetPwWindow::resetPwWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::resetPwWindow)
{
    ui->setupUi(this);
}

resetPwWindow::~resetPwWindow()
{
    delete ui;
}
