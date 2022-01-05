#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QVector>

class Common : public QObject
{
    Q_OBJECT
public:
    Common();
    ~Common();

    struct Coords
    {
        qint16 col = 0;
        qint16 row = 0;
    };
    static void setRandomSeed();
    static Coords intToCoords(const quint16&, const quint16&);
    static quint16 CoordsToInt(const Coords&, const quint16&);
    static QVector<quint16> randomShuffle(const quint16&, const quint16&);
public slots:
    void print_debug_slot(const QString&);
};

#endif // COMMON_H
