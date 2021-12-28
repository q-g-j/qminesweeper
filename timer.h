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
    ~Timer();
    int counterFine = 0;
    void timerStart();
    void timerStop();
private:
    int counter = 0;
    QTimer *timerInstance;
    QLabel *timerSeconds;
    QLabel *timerTenSeconds;
    QLabel *timerMinutes;
    QLabel *timerTenMinutes;

private slots:
    void timer_slot();
};

#endif // TIMER_H
