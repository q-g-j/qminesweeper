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
    void setRandomSeed();
    Coords intToStruct(int&, int&);
    int structToInt(Coords&, int&);
};

#endif // COMMON_H
