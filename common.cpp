#include <QDebug>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QEventLoop>
#include <random>

#include "common.h"

quint32 Common::counterFine = 0;
quint32 Common::timerTenMinutes = 0;
quint32 Common::timerMinutes = 0;
quint32 Common::timerTenSeconds = 0;
quint32 Common::timerSeconds = 0;

Common::Common() {}

Common::~Common() {}

QVector<quint32> Common::randomShuffle(
        const qint32& high,
        const quint32& userFirstInput
        )
{
    std::random_device rd;
    std::mt19937 generator(rd());

    QVector<quint32> returnVector;

    for (quint32 i = 1; returnVector.size() < high - 1; i++)
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
        const quint32& position,
        const quint32& cols
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
        const quint32& cols
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
    Common::counterFine = currentTimer_.counterFine;
    Common::timerTenMinutes = currentTimer_.tenMinutes;
    Common::timerMinutes = currentTimer_.minutes;
    Common::timerTenSeconds = currentTimer_.tenSeconds;
    Common::timerSeconds = currentTimer_.seconds;
}

void Common::print_debug_slot(const QString& debugMessage)
{
    quint32 tenthSeconds = (Common::counterFine - (Common::counterFine / 1000) * 1000) / 100;
    quint32 hundredthSeconds = (Common::counterFine - (Common::counterFine / 100) * 100) / 10;

    qDebug() << QString::number(Common::timerTenMinutes)
                + QString::number(Common::timerMinutes) + "m"
                + QString::number(Common::timerTenSeconds)
                + QString::number(Common::timerSeconds) + "s"
                + QString::number(tenthSeconds)
                + QString::number(hundredthSeconds)
                + QString::number(0) + "ms" + " " + debugMessage;
}
