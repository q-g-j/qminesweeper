#ifndef SOLVER_H
#define SOLVER_H

#include <QObject>

#include <field.h>

class Solver : public QObject
{
    Q_OBJECT
private:

public:
    Solver(QObject *parent = nullptr);
    ~Solver();

    // public methods:
    void autoSolve(Field&, bool, bool, bool);

signals:
    void place_remove_flag_signal(const Common::Coords&);
};

#endif // SOLVER_H
