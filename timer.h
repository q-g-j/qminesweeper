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
    quint32  counterFine = 0;
    void timerStart();
    void timerStop();
private:
    quint16 counter = 0;
    QTimer *timerInstance = nullptr;
private slots:
    void timer_slot();
signals:
    void set_infobar_time_signal(const QString&, const quint16&);
};

#endif // TIMER_H
