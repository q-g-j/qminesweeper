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
        quint8 tenMinutes = 0;
        quint8 minutes = 0;
        quint8 tenSeconds = 0;
        quint8 seconds = 0;
        quint32 counterFine = 0;
    };

    TimerStruct currentTimer;

private:
    QTimer *timerInstance = nullptr;
    void timerWorker();
private slots:
    void timer_slot();
signals:
    void current_timer_signal(const TimerStruct&);
    void print_debug_signal(const QString&);
};

#endif // TIMER_H
