#include <QTimer>
#include <QEventLoop>

#include "mainwindow.h"
#include "sleep.h"

Sleep::Sleep(std::function<void()> funct_, const quint16 &milliseconds_)
    : QObject {}
{
    this->milliseconds = milliseconds_;
    this->funct = funct_;
}

Sleep::~Sleep() {}

void Sleep::sleep()
{
    QEventLoop loop;
    QTimer::singleShot(this->milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
    this->funct();
    emit this->finished();
}
