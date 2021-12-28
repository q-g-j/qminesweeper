#ifndef SOLVER_H
#define SOLVER_H

#include <QObject>

#include <field.h>

class Solver : public QObject
{
    Q_OBJECT
private:
    // private methods:
    bool cancelOnNewGameRequested();
public:
    Solver(QObject *parent = nullptr);
    ~Solver();

    // public variables:
    bool isSolverRunning;
    bool isNewGameRequestedFromMenu;
    bool isNewGameRequestedFromSmiley;

    // public methods:
    void autoSolve(Field&, bool, bool, bool);
signals:
    void place_flag_signal(const Common::Coords&);
    void solver_stopped_signal(const char&);
};

#endif // SOLVER_H
