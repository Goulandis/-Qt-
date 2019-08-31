#ifndef RESETPWWINDOW_H
#define RESETPWWINDOW_H

#include <QWidget>

namespace Ui {
class resetPwWindow;
}

class resetPwWindow : public QWidget
{
    Q_OBJECT

public:
    explicit resetPwWindow(QWidget *parent = 0);
    ~resetPwWindow();

private:
    Ui::resetPwWindow *ui;
};

#endif // RESETPWWINDOW_H
