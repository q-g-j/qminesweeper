#ifndef TIMER_H
#define TIMER_H

#include <QLabel>
#include <QObject>
#include <QTimer>

class Timer : public QObject
{
    Q_OBJECT
public:
    Timer(QLabel* = nullptr, QLabel* = nullptr, QLabel* = nullptr, QLabel* = nullptr);
    QTimer *timerInstance;
    int counterFine = 0;
private:
    int counter = 0;
    QLabel *timerSeconds;
    QLabel *timerTenSeconds;
    QLabel *timerMinutes;
    QLabel *timerTenMinutes;

private slots:
    void TimerSlot();
};

#endif // TIMER_H
