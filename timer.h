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
    qint32  counterFine = 0;
    void timerStart();
    void timerStop();
private:
    qint32 counter = 0;
    QTimer *timerInstance = nullptr;
private slots:
    void timer_slot();
signals:
    void set_infobar_time_signal(const QString&, const qint32&);
};

#endif // TIMER_H
