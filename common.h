#ifndef COMMON_H
#define COMMON_H

#include <QString>

#include "button.h"

class Common
{
public:
    struct Coords
    {
        int col = 0;
        int row = 0;
    };
    static void setRandomSeed();
    static Coords intToCoords(const int&, const int&);
    static int CoordsToInt(const Coords&, const int&);
    static void sleep(const int&);
    static void setIcon(Button*, const int&, const QString&);
};

#endif // COMMON_H
