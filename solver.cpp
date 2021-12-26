#include <QVector>
#include <QDebug>

#include <common.h>
#include <solver.h>


Solver::Solver(QObject(*parent)) : QObject(parent)
{}

Solver::~Solver()
{}

void Solver::autoSolve(Field& field, bool doPlaceFlags, bool doFlagAutoReveal, bool doSolve)
{
    QVector<int> poolCoveredVector;

    if (doPlaceFlags)
    {
        // for each button in field2DVector:
        for (int i = 1; i <= field.cols; i++)
        {
            for (int j = 1; j <= field.rows; j++)
            {
                Common::Coords tempCoords;
                tempCoords.col = i;
                tempCoords.row = j;

                if (field.field2DVector[tempCoords.col][tempCoords.row] != '0')
                {
                    // if it's a number:
                    if (field.isNumber(tempCoords))
                    {
                        // create 2 vectors:
                        // one for covered neighbors,
                        // one for neighbor flags
                        QVector<Common::Coords> coveredVector;
                        coveredVector = field.findNeighbours(field.field2DVector, tempCoords, ' ');
                        QVector<Common::Coords> flagsVector;
                        flagsVector = field.findNeighbours(field.field2DVector, tempCoords, 'F');

                        // if the number of covered neighbors plus the number of neighbor flags matches the current buttons number,
                        // add the covered buttons to poolCoveredVector:
                        if ((flagsVector.size() + coveredVector.size()) == field.field2DVector[tempCoords.col][tempCoords.row] - 48)
                        {
                            for (int k = 0; k < coveredVector.size(); k++)
                            {
                                int tempPosition = Common::CoordsToInt(coveredVector.at(k), field.cols);
                                if (std::find(poolCoveredVector.begin(), poolCoveredVector.end(), tempPosition) == poolCoveredVector.end())
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
            for (int i = 0; i < poolCoveredVector.size(); i++)
            {
                Common::sleep(40);
                Common::Coords tempCoords;
                tempCoords = Common::intToCoords(poolCoveredVector.at(i), field.cols);
                emit this->place_remove_flag_signal(tempCoords);
            }
        }
    }
    // run field.flagAutoReveal() on all buttons:
    if (doFlagAutoReveal)
    {
        for (int i = 1; i <= field.rows; ++i)
        {
            for (int j = 1; j <= field.cols; ++j)
            {
                Common::Coords tempCoords;
                tempCoords.row = i;
                tempCoords.col = j;
                field.flagAutoReveal(tempCoords, true, true);
            }
        }
    }

    // re-run the whole function, if there are buttons in poolCoveredVector or
    // if it's the first run of this function when "s" or "S" (= "solve") was hit:
    if (poolCoveredVector.size() || doSolve)
    {
        autoSolve(field, doPlaceFlags, doFlagAutoReveal, false);
    }
}
