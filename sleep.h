#ifndef SLEEP_H
#define SLEEP_H

#include <QObject>
#include <functional>

class Sleep : public QObject
{
    Q_OBJECT
public:
    Sleep(std::function<void()>, const quint16&);
    ~Sleep();

public slots:
    void sleep();
signals:
    void finished();
private:
    quint16 milliseconds;

    std::function<void()> funct;
};

#endif // SLEEP_H
