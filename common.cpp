#include <QDebug>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QRandomGenerator>
#include <QEventLoop>

#include "common.h"

Common::Common()
{}

Common::~Common()
{}

QVector<quint16> Common::randomShuffle(const quint16& high, const quint16& userFirstInput)
{
    QVector<quint16> returnVector;
    while (returnVector.size() < high - 1)
    {
        for (quint16 i = 1; i < high; i++)
        {
            quint16 value = QRandomGenerator::global()->bounded(high) + 1;
            if (value != userFirstInput)
            {
                if (std::find(returnVector.begin(), returnVector.end(), value) == returnVector.end())
                {
                    returnVector.append(value);
                }
            }
        }
    }
    return returnVector;
}

// convert coords in type integer to coords in type struct (e.g. position = 4 will return coords.col = 4, coords.row = 1):
Common::Coords Common::intToCoords(const quint16& position, const quint16& cols)
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
quint16 Common::CoordsToInt(const Common::Coords& coords, const quint16& cols)
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

void Common::sleep(const quint16& milliseconds)
{
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}

void Common::print_debug_slot(const QString& from)
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "-" << from;
}
