#include <QTimer>
#include <QEventLoop>

#include "mainwindow.h"
#include "sleep.h"

Sleep::Sleep(const quint16 &milliseconds_)
    : QObject {}
{
    this->milliseconds = milliseconds_;
}

Sleep::Sleep(QThread *thread_, const quint16 &milliseconds_)
    : QObject {}
{
    this->thread = thread_;
    this->milliseconds = milliseconds_;
    connect(thread_, &QThread::started, this, &Sleep::threadedSleep);
    connect(this, &Sleep::finished, thread_, &QThread::quit);
    connect(this, &Sleep::finished, this, &Sleep::deleteLater);
    connect(thread_, &QThread::finished, thread_, &QThread::deleteLater);
    thread_->start();
}

Sleep::~Sleep() {}

void Sleep::sleep(const quint16& milliseconds)
{
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}

void Sleep::threadedSleep()
{
    QEventLoop loop;
    QTimer::singleShot(this->milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
    emit this->finished();
}
