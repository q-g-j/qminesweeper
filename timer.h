#ifndef TIMER_H
#define TIMER_H

#include <QLabel>
#include <QObject>
#include <QTimer>

class Timer : public QObject
{
    Q_OBJECT
public:
    Timer();
    ~Timer();
    int counterFine = 0;
    void timerStart();
    void timerStop();
private:
    int counter = 0;
    QTimer *timerInstance = nullptr;
private slots:
    void timer_slot();
signals:
    void set_infobar_time_signal(const QString&, const int&);
};

#endif // TIMER_H
