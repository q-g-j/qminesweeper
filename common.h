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
        qint32 col = 0;
        qint32 row = 0;
    };
    static Coords intToCoords(const qint32&, const qint32&);
    static qint32 CoordsToInt(const Coords&, const qint32&);
    static QVector<qint32> randomShuffle(const qint32&, const qint32&);
public slots:
    void print_debug_slot(const QString&);
};

#endif // COMMON_H
