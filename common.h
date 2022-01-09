#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QPixmap>
#include <QVector>

#include "debug.h"
#include "timer.h"

class Common : public QObject
{
    Q_OBJECT
public:
    Common();
    ~Common();

    static quint32 counterFine;
    static quint32 timerTenMinutes;
    static quint32 timerMinutes;
    static quint32 timerTenSeconds;
    static quint32 timerSeconds;

    struct Coords
    {
        qint32 col = 0;
        qint32 row = 0;
    };

    static Coords intToCoords(const quint32&, const quint32&);
    static qint32 CoordsToInt(const Coords&, const quint32&);
    static QVector<quint32> randomShuffle(const qint32&, const quint32&);
public slots:
    static void current_timer_slot(const Timer::TimerStruct&);
    static void print_debug_slot(const QString&);
};

#endif // COMMON_H
