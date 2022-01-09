#include <QDebug>

#include "timer.h"

Timer::Timer()
{
    this->timerInstance = new QTimer(this);
    this->timerInstance->setTimerType(Qt::PreciseTimer);

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

void Timer::timerWorker()
{
#ifdef DEBUG_TIMER_MORE_ACCURATE
    quint32 counterSeconds = this->currentTimer.counterFine / 100;
#else
    quint32 counterSeconds = this->currentTimer.counterFine;
#endif

    this->currentTimer.seconds = counterSeconds % 10;
    if (counterSeconds < 60)
    {
        this->currentTimer.tenSeconds = (counterSeconds - (counterSeconds % 10)) / 10;
    }
    else
    {
        this->currentTimer.tenSeconds = ((counterSeconds % 60) - ((counterSeconds % 60) % 10)) / 10;
    }
    this->currentTimer.minutes = (counterSeconds / 60) % 10;
    if ((counterSeconds / 60) < 99)
    {
        this->currentTimer.tenMinutes = ((counterSeconds / 60) - (counterSeconds / 60) % 10) / 10;
    }
    else
    {
        this->currentTimer.tenMinutes = 9;
    }

    emit this->current_timer_signal(this->currentTimer);

    if (counterSeconds < 599900)
    {
        this->currentTimer.counterFine++;
    }
}

void Timer::timerStart()
{
#ifdef DEBUG_TIMER_MORE_ACCURATE
    this->timerInstance->start(10);
#else
    this->timerInstance->start(1000);
#endif
    this->timerWorker();
}

void Timer::timerStop()
{
    this->timerInstance->stop();
}

void Timer::timer_slot()
{
    this->timerWorker();
}
