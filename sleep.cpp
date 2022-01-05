#include <QTimer>
#include <QEventLoop>

#include "sleep.h"

/*
Usage for the threaded sleep:
    In the calling class:

    1. Create a slot that will be executed after sleep

    2. Create a new Thread and a new instance of Sleep (no need to call delete):
        quint32 milliseconds = 500;
        QThread* thread = new QThread();
        Sleep* sleep = new Sleep(thread, milliseconds);
        sleep->moveToThread(thread);
        connect(sleep, &Sleep::finished, this, &CallingClass::execute_after_sleep_slot);

Usage for the normal sleep:
    Just call the static function Sleep::sleep(const quint16&):
        Sleep::sleep(milliseconds)
*/

Sleep::Sleep(QThread *thread_, const quint32 &milliseconds_)
    : QObject {}
{
    this->thread = thread_;
    this->milliseconds = milliseconds_;
    connect(thread_, &QThread::started, this, &Sleep::threaded_sleep_private_slot);
    connect(this, &Sleep::finished, thread_, &QThread::quit);
    connect(this, &Sleep::finished, this, &Sleep::deleteLater);
    connect(thread_, &QThread::finished, thread_, &QThread::deleteLater);
    thread_->start();
}

Sleep::~Sleep() {}

void Sleep::sleep(const quint32& milliseconds)
{
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}

void Sleep::threaded_sleep_private_slot()
{
    QEventLoop loop;
    QTimer::singleShot(this->milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
    emit this->finished();
}
