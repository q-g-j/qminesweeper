#ifndef TIMER_H
#define TIMER_H

#include <QLabel>
#include <QObject>
#include <QTimer>

#include "debug.h"

class Timer : public QObject
{
    Q_OBJECT
public:
    Timer();
    ~Timer();
    void timerStart();
    void timerStop();

    struct TimerStruct
    {
        quint32 tenMinutes = 0;
        quint32 minutes = 0;
        quint32 tenSeconds = 0;
        quint32 seconds = 0;
        quint32 counterFine = 0;
    };

    TimerStruct currentTimer;

private:
    QTimer *timerInstance = nullptr;
private slots:
    void timer_slot();
signals:
    void current_timer_signal(const TimerStruct&);
};

#endif // TIMER_H
