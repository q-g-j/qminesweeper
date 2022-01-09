#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QPixmap>
#include <QVector>

#include "timer.h"

class Common : public QObject
{
    Q_OBJECT
public:
    Common();
    ~Common();

    Timer::TimerStruct currentTimer;

    struct Coords
    {
        qint32 col = 0;
        qint32 row = 0;
    };

    static Coords intToCoords(const qint32&, const qint32&);
    static qint32 CoordsToInt(const Coords&, const qint32&);
    static QVector<qint32> randomShuffle(const qint32&, const qint32&);
public slots:
    void current_timer_slot(const Timer::TimerStruct&);
    void print_debug_slot(const QString&);
};

#endif // COMMON_H
