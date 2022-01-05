#include <QVector>
#include <QDebug>
#include <QThread>

#include <common.h>
#include <solver.h>
#include <sleep.h>


Solver::Solver() {}

Solver::~Solver() {}

void Solver::autoSolve(
        Field& field,
        bool doPlaceFlags,
        bool doFlagAutoReveal,
        bool doSolve
        )
{
    this->isSolverRunning = true;
    emit this->is_solver_running_signal(true);

    if (this->cancelOnNewGameRequested() == true) return;

    QVector<quint16> poolCoveredVector;

    if (doPlaceFlags == true)
    {
        // for each button in field2DVector:
        for (quint16 i = 1; i <= field.cols; i++)
        {
            for (quint16 j = 1; j <= field.rows; j++)
            {
                if (this->cancelOnNewGameRequested() == true) return;

                Common::Coords tempCoords;
                tempCoords.col = i;
                tempCoords.row = j;

                if (field.field2DVector[tempCoords.col][tempCoords.row] != '0')
                {
                    // if it's a number:
                    if (field.isNumber(tempCoords) == true)
                    {
                        // create 2 vectors:
                        // one for covered neighbors,
                        // one for neighbor flags
                        QVector<Common::Coords> coveredVector;
                        coveredVector = field.findNeighbours(
                                    field.field2DVector,
                                    tempCoords,
                                    ' '
                                    );
                        QVector<Common::Coords> flagsVector;
                        flagsVector = field.findNeighbours(
                                    field.field2DVector,
                                    tempCoords,
                                    'F'
                                    );

                        // if the number of covered neighbors plus the number of neighbor flags matches the current buttons number,
                        // add the covered buttons to poolCoveredVector:
                        if ((flagsVector.size() + coveredVector.size()) == field.field2DVector[tempCoords.col][tempCoords.row] - 48)
                        {
                            for (quint16 k = 0; k < coveredVector.size(); k++)
                            {
                                if (this->cancelOnNewGameRequested()) return;

                                quint16 tempPosition = Common::CoordsToInt(coveredVector.at(k), field.cols);
                                if (
                                        std::find(
                                            poolCoveredVector.begin(),
                                            poolCoveredVector.end(),
                                            tempPosition
                                            ) == poolCoveredVector.end()
                                        )
                                {
                                    poolCoveredVector.push_back(tempPosition);
                                }
                            }
                        }
                    }
                }
            }
        }

        // place a flag at the coords of each element of poolCoveredVector:
        if (poolCoveredVector.size() != 0)
        {
            for (quint16 i = 0; i < poolCoveredVector.size(); i++)
            {
                if (this->cancelOnNewGameRequested() == true) return;

                Sleep::sleep(30);
                Common::Coords tempCoords;
                tempCoords = Common::intToCoords(
                            poolCoveredVector.at(i),
                            field.cols
                            );
                emit this->solver_place_flag_signal(tempCoords);
            }
        }
    }

    // run field.flagAutoReveal() on all buttons:
    if (doFlagAutoReveal == true)
    {
        for (quint16 i = 1; i <= field.rows; ++i)
        {
            for (quint16 j = 1; j <= field.cols; ++j)
            {
                if (this->cancelOnNewGameRequested() == true) return;

                Common::Coords tempCoords;
                tempCoords.row = i;
                tempCoords.col = j;
                field.flagAutoReveal(
                            tempCoords,
                            true,
                            true
                            );
            }
        }
    }

    // re-run the whole function, if there are buttons in poolCoveredVector or
    // if it's the first run of this function when "s" or "S" (= "solve") was hit:
    if (
            this->isNewGameRequestedFromMenu == false
            && this->isNewGameRequestedFromSmiley == false
            && (poolCoveredVector.size() != 0 || doSolve  == true)
            )
    {
        autoSolve(field, doPlaceFlags, doFlagAutoReveal, false);
    }
    else
    {
        this->isSolverRunning = false;
        emit this->is_solver_running_signal(false);
        return;
    }
}

bool Solver::cancelOnNewGameRequested()
{
    if (this->isNewGameRequestedFromMenu == true)
    {
        emit solver_stopped_signal('m');
        return true;
    }
    else if (this->isNewGameRequestedFromSmiley == true)
    {
        emit solver_stopped_signal('s');
        return true;
    }
    else return false;
}
