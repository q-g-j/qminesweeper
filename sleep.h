#ifndef SLEEP_H
#define SLEEP_H

#include <QObject>
#include <QThread>

class Sleep : public QObject
{
    Q_OBJECT
public:
    Sleep(QThread*, const quint32&);
    ~Sleep();

    static void sleep(const quint32&);
private slots:
    void threaded_sleep_private_slot();
signals:
    void finished();
private:
    QThread *thread;
    quint16 milliseconds;
};

#endif // SLEEP_H
