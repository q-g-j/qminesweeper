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
    static Coords intToStruct(int&, int&);
    static int structToInt(Coords&, int&);
};

#endif // COMMON_H
