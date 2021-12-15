#ifndef COMMON_H
#define COMMON_H

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
};

#endif // COMMON_H
