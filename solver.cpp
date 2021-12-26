// system headers:
#include <algorithm>
#include <atomic>
#include <iostream>
#include <QVector>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// project headers:
#include <common.h>
#include <solver.h>

void Solver::autoSolve(Field& field, const int& cols, const int& rows, bool doPlaceFlags, bool doFlagAutoReveal, bool doSolve)
{
    QVector<int> poolCoveredVector;

    if (doPlaceFlags)
    {
        // for each button in field2DVector:
        for (int i = 1; i <= cols; ++i)
        {
            for (int j = 1; j <= cols; ++j)
            {
                Common::Coords tempCoords;
                tempCoords.row = i;
                tempCoords.col = j;

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
                        for (int k = 0; k < coveredVector.size(); ++k)
                        {
                            if (std::find(poolCoveredVector.begin(), poolCoveredVector.end(), Common::CoordsToInt(coveredVector.at(k), cols)) == poolCoveredVector.end())
                            {
                                poolCoveredVector.push_back(Common::CoordsToInt(coveredVector.at(k), cols));
                            }
                        }
                    }
                }
            }
        }

        // place a flag at the coords of each element of poolCoveredVector:
        if (poolCoveredVector.size() != 0)
        {
            for (int i = 0; i < poolCoveredVector.size(); ++i)
            {
                Common::Coords tempCoords;
                tempCoords = Common::intToCoords(poolCoveredVector.at(i), cols);

                // trigger field slot "place_flag_slot"
            }
        }
    }
    // run field.flagAutoReveal() on all buttons:
    if (doFlagAutoReveal)
    {
        for (int i = 1; i <= rows; ++i)
        {
            for (int j = 1; j <= cols; ++j)
            {
                Common::Coords tempCoords;
                tempCoords.row = i;
                tempCoords.col = j;
                field.flagAutoReveal(tempCoords);
            }
        }
    }

    // re-run the whole function, if there are buttons in poolCoveredVector or
    // if it's the first run of this function when "s" or "S" (= "solve") was hit:
    if (poolCoveredVector.size() != 0 || doSolve == true)
    {
        autoSolve(field, cols, rows, doPlaceFlags, doFlagAutoReveal, false);
    }
}
