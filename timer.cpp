#include <QDebug>

#include "timer.h"

Timer::Timer(QLabel *timerSeconds_, QLabel *timerTenSeconds_, QLabel *timerMinutes_, QLabel *timerTenMinutes_)
{
    this->timerSeconds = timerSeconds_;
    this->timerTenSeconds = timerTenSeconds_;
    this->timerMinutes = timerMinutes_;
    this->timerTenMinutes = timerTenMinutes_;
    this->timerInstance = new QTimer(this);
    this->timerInstance->setTimerType(Qt::PreciseTimer);

    this->timerInstance->stop();

    connect(this->timerInstance, SIGNAL(timeout()), this, SLOT(timer_slot()));

    this->timerSeconds->setText(QString::number(0));
    this->timerTenSeconds->setText(QString::number(0));
    this->timerMinutes->setText(QString::number(0));
    this->timerTenMinutes->setText(QString::number(0));
}

Timer::~Timer()
{
    delete this->timerInstance;
    this->timerInstance = nullptr;
}

void Timer::timer_slot()
{
    if (this->counterFine % 100 == 0)
    {
        this->counter = this->counterFine / 100;
        int seconds, tenSeconds, minutes, tenMinutes;
        seconds = this->counter % 10;
        if (this->counter < 60)
        {
            tenSeconds = (this->counter - (this->counter % 10)) / 10;
        }
        else
        {
            tenSeconds = ((this->counter % 60) - ((this->counter % 60) % 10)) / 10;
        }
        minutes = (this->counter / 60) % 10;
        if ((this->counter / 60) < 99)
        {
            tenMinutes = ((this->counter / 60) - (this->counter / 60) % 10) / 10;
        }
        else
        {
            tenMinutes = 9;
        }
        this->timerSeconds->setText(QString::number(seconds));
        this->timerTenSeconds->setText(QString::number(tenSeconds));
        this->timerMinutes->setText(QString::number(minutes));
        this->timerTenMinutes->setText(QString::number(tenMinutes));
    }
    if (this->counterFine < 599900)
    {
        this->counterFine++;
    }
}

void Timer::timerStart()
{
    this->timerInstance->start(10);
}

void Timer::timerStop()
{
    this->timerInstance->stop();
}
