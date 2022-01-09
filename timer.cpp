#include <QDebug>

#include "timer.h"

Timer::Timer()
{
    this->timerInstance = new QTimer(this);
    this->timerInstance->setTimerType(Qt::PreciseTimer);

    this->timerInstance->stop();

    connect(this->timerInstance, SIGNAL(timeout()), this, SLOT(timer_slot()));

    emit this->current_timer_signal(this->currentTimer);
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
    quint32 counter = this->currentTimer.counterFine / 100;

    this->currentTimer.seconds = counter % 10;
    if (counter < 60)
    {
        this->currentTimer.tenSeconds = (counter - (counter % 10)) / 10;
    }
    else
    {
        this->currentTimer.tenSeconds = ((counter % 60) - ((counter % 60) % 10)) / 10;
    }
    this->currentTimer.minutes = (counter / 60) % 10;
    if ((counter / 60) < 99)
    {
        this->currentTimer.tenMinutes = ((counter / 60) - (counter / 60) % 10) / 10;
    }
    else
    {
        this->currentTimer.tenMinutes = 9;
    }

    emit this->current_timer_signal(this->currentTimer);

    if (this->currentTimer.counterFine < 599900)
    {
        this->currentTimer.counterFine++;
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
