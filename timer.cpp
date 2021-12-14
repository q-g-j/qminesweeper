#include <QDebug>
#include <iostream>

#include "timer.h"

Timer::Timer(QLabel *timerSeconds_, QLabel *timerTenSeconds_, QLabel *timerMinutes_, QLabel *timerTenMinutes_)
{
    timerSeconds = timerSeconds_;
    timerTenSeconds = timerTenSeconds_;
    timerMinutes = timerMinutes_;
    timerTenMinutes = timerTenMinutes_;
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);

    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
    timer->start(10);
    timerSeconds->setText(QString::number(0));
    timerTenSeconds->setText(QString::number(0));
    timerMinutes->setText(QString::number(0));
    timerTenMinutes->setText(QString::number(0));
}

void Timer::TimerSlot()
{
    if (this->counterFine < 599900 && this->keepGoing == true)
        this->counterFine = (this->counterFine + 1);
    if (this->counterFine % 100 == 0)
    {
        this->counter = this->counterFine / 100;
        int seconds, tenSeconds, minutes, tenMinutes;
        seconds = this->counter % 10;
        if (this->counter < 60)
            tenSeconds = (this->counter - (this->counter % 10)) / 10;
        else
            tenSeconds = ((this->counter % 60) - ((this->counter % 60) % 10)) / 10;
        minutes = (this->counter / 60) % 10;
        if ((this->counter / 60) < 99)
            tenMinutes = ((this->counter / 60) - (this->counter / 60) % 10) / 10;
        else
            tenMinutes = 9;
        timerSeconds->setText(QString::number(seconds));
        timerTenSeconds->setText(QString::number(tenSeconds));
        timerMinutes->setText(QString::number(minutes));
        timerTenMinutes->setText(QString::number(tenMinutes));
    }
}


