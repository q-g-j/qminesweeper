#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#else
    #include <random>
    #include "time.h"
#endif

#include <QThread>
#include <QTimer>
#include <QEventLoop>

#include "common.h"

// needed for random_shuffle() (place the mines):
void Common::setRandomSeed()
{
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        srand(GetTickCount());
    #else
        srand(time(NULL));
    #endif
}

// convert coords in type integer to coords in type struct (e.g. position = 4 will return coords.col = 4, coords.row = 1):
Common::Coords Common::intToCoords(const int& position, const int& cols)
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
int Common::CoordsToInt(const Common::Coords& coords, const int& cols)
{
    if (coords.row == 1)
        return coords.col;
    else
        return cols * (coords.row - 1) + coords.col;
}

void Common::sleep(const int& milliseconds)
{
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}
