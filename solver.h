#ifndef SOLVER_H
#define SOLVER_H

#include <field.h>

class Solver
{
private:

public:
    Solver();
    ~Solver();

    // public methods:
    static void autoSolve(Field&, const int&, const int&, bool, bool, bool);
};

#endif // SOLVER_H
