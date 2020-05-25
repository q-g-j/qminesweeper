#include "time.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <fcntl.h>
    #include <io.h>
    #include <stdio.h>
    #include <windows.h>
#else
    #include <random>
#endif

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
Common::Coords Common::intToStruct(int& position, int& cols)
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
int Common::structToInt(Common::Coords& coords, int& cols)
{
    int position = 0;
    if (coords.row == 1)
        return position = coords.col;
    else
        return position = (cols) * (coords.row-1) + coords.col;
}
