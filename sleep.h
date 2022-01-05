#ifndef SLEEP_H
#define SLEEP_H

#include <QObject>
#include <QThread>

class Sleep : public QObject
{
    Q_OBJECT
public:
    Sleep(const quint16&);
    Sleep(QThread*, const quint16&);
    ~Sleep();

    static void sleep(const quint16&);
public slots:
    void threadedSleep();
signals:
    void finished();
private:
    QThread *thread;
    quint16 milliseconds;
};

#endif // SLEEP_H
