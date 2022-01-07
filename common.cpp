#include <QDebug>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QEventLoop>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#else
    #include <random>
    #include "time.h"
#endif

#include "common.h"

Common::Common() {}

Common::~Common() {}
// needed for random_shuffle() (place the mines):

void Common::setRandomSeed()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        srand(GetTickCount());
    #else
        srand(time(NULL));
    #endif
}

QVector<qint32> Common::randomShuffle(
        const qint32& high,
        const qint32& userFirstInput
        )
{
    QVector<qint32> returnVector;

    for (qint32 i = 1; returnVector.size() < high - 1; i++)
    {
        if (i != userFirstInput)
        {
            returnVector.push_back(i);
        }
    }

    std::random_shuffle(returnVector.begin(), returnVector.end());

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

void Common::print_debug_slot(const QString& from)
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "-" << from;
}
