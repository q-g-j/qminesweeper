#include <QDebug>

#include "timer.h"

Timer::Timer()
{
    this->timerInstance = new QTimer(this);
    this->timerInstance->setTimerType(Qt::PreciseTimer);

    this->timerInstance->stop();

    connect(this->timerInstance, SIGNAL(timeout()), this, SLOT(timer_slot()));

    emit this->set_infobar_time_signal("seconds", 0);
    emit this->set_infobar_time_signal("tenSeconds", 0);
    emit this->set_infobar_time_signal("minutes", 0);
    emit this->set_infobar_time_signal("tenMinutes", 0);
}

Timer::~Timer()
{
    if (this->timerInstance != nullptr)
    {
        delete this->timerInstance;
        this->timerInstance = nullptr;
    }
}

void Timer::timer_slot()
{
    if (this->counterFine % 100 == 0)
    {
        this->counter = this->counterFine / 100;
    quint16 seconds, tenSeconds, minutes, tenMinutes;
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
        emit this->set_infobar_time_signal("seconds", seconds);
        emit this->set_infobar_time_signal("tenSeconds", tenSeconds);
        emit this->set_infobar_time_signal("minutes", minutes);
        emit this->set_infobar_time_signal("tenMinutes", tenMinutes);
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
