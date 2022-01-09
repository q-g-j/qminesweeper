#include <QDebug>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QEventLoop>
#include <random>

#include "common.h"

Common::Common()
{
    this->currentTimer.tenMinutes = 0;
    this->currentTimer.minutes = 0;
    this->currentTimer.tenSeconds = 0;
    this->currentTimer.seconds = 0;
    this->currentTimer.counterFine = 0;
}

Common::~Common() {}

QVector<qint32> Common::randomShuffle(
        const qint32& high,
        const qint32& userFirstInput
        )
{
    std::random_device rd;
    std::mt19937 generator(rd());

    QVector<qint32> returnVector;

    for (qint32 i = 1; returnVector.size() < high - 1; i++)
    {
        if (i != userFirstInput)
        {
            returnVector.push_back(i);
        }
    }

    std::shuffle(returnVector.begin(), returnVector.end(), generator);

    return returnVector;
}

// convert coords in type integer to coords in type struct (e.g. position = 4 will return coords.col = 4, coords.row = 1):
Common::Coords Common::intToCoords(
        const qint32& position,
        const qint32& cols
        )
{
    Common::Coords coords;

    if (position <= cols)
    {
        coords.col = position;
        coords.row = 1;
    }
    else if (position % cols == 0)
    {
        coords.col = cols;
        coords.row = position / cols;
    }
    else
    {
        coords.col = position % cols;
        coords.row = position / cols + 1;
    }
    return coords;
}

// the above function the other way around
qint32 Common::CoordsToInt(
        const Common::Coords& coords,
        const qint32& cols
        )
{
    if (coords.row == 1)
    {
        return coords.col;
    }
    else
    {
        return cols * (coords.row - 1) + coords.col;
    }
}

void Common::current_timer_slot(const Timer::TimerStruct& currentTimer_)
{
    this->currentTimer = currentTimer_;
}

void Common::print_debug_slot(const QString& debugMessage)
{
    quint32 tenthSeconds = 0;
    quint32 hundredthSeconds = 0;
    tenthSeconds = (this->currentTimer.counterFine - (this->currentTimer.counterFine / 1000) * 1000) / 100;
    hundredthSeconds = (this->currentTimer.counterFine - (this->currentTimer.counterFine / 100) * 100) / 10;

    qDebug() << QString::number(this->currentTimer.tenMinutes)
                + QString::number(this->currentTimer.minutes) + "m"
                + QString::number(this->currentTimer.tenSeconds)
                + QString::number(this->currentTimer.seconds) + "s"
                + QString::number(tenthSeconds)
                + QString::number(hundredthSeconds)
                + QString::number(0) + "ms" + " " + debugMessage;
}
