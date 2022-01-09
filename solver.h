#ifndef SOLVER_H
#define SOLVER_H

#include <QObject>

#include "debug.h"
#include <field.h>

class Solver : public QObject
{
    Q_OBJECT
private:
    // private methods:
    bool cancelOnNewGameRequested();
public:
    Solver();
    ~Solver();

    // public variables:
    bool isSolverRunning = false;
    bool isNewGameRequestedFromMenu = false;
    bool isNewGameRequestedFromSmiley = false;

    // public methods:
    void autoSolve(Field&, bool, bool, bool);
signals:
    void solver_place_flag_signal(const Common::Coords&);
    void solver_stopped_signal(const char&);
    void is_solver_running_signal(bool);
    void print_debug_signal(const QString&);
};

#endif // SOLVER_H
