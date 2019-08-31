#include "delay.h"
#include <QTime>

Delay::Delay()
{

}

Delay::Delay(unsigned int msec)
{
    QTime reachTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < reachTime) {
        ;
    }
}
