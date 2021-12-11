#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSizePolicy>
#include <QGridLayout>

#include <QVector>

#include "cell.h"
#include "common.h"
#include "field.h"
#include "mainwindow.h"

Field::Field(QWidget *parent, const int& cols, const int& rows, const int& mines, const int& cellSize) : QWidget(parent)
{
    this->firstTurn = true;
    this->cols = cols;
    this->rows = rows;
    this->mines = mines;
    this->cellSize = cellSize;
    this->fieldArray = createArray();
    this->minesArray = createArray();
    this->minesLeft = mines;
    this->countCovered = cols * rows;
    this->gameover = false;
    createCell();

    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    QFile file_button_flag      (":/stylesheet/button_flag.qss");
    QFile file_button_mine      (":/stylesheet/button_mine.qss");
    QFile file_button_mine_hit  (":/stylesheet/button_mine_hit.qss");
    QFile file_button_covered   (":/stylesheet/button_covered.qss");
    QFile file_button_uncovered (":/stylesheet/button_uncovered.qss");
    QFile file_button_1         (":/stylesheet/button_1.qss");
    QFile file_button_2         (":/stylesheet/button_2.qss");
    QFile file_button_3         (":/stylesheet/button_3.qss");
    QFile file_button_4         (":/stylesheet/button_4.qss");
    QFile file_button_5         (":/stylesheet/button_5.qss");
    QFile file_button_6         (":/stylesheet/button_6.qss");
    QFile file_button_7         (":/stylesheet/button_7.qss");
    QFile file_button_8         (":/stylesheet/button_8.qss");
    file_button_flag.open       (QFile::ReadOnly);
    file_button_mine.open       (QFile::ReadOnly);
    file_button_mine_hit.open   (QFile::ReadOnly);
    file_button_covered.open    (QFile::ReadOnly);
    file_button_uncovered.open  (QFile::ReadOnly);
    file_button_1.open          (QFile::ReadOnly);
    file_button_2.open          (QFile::ReadOnly);
    file_button_3.open          (QFile::ReadOnly);
    file_button_4.open          (QFile::ReadOnly);
    file_button_5.open          (QFile::ReadOnly);
    file_button_6.open          (QFile::ReadOnly);
    file_button_7.open          (QFile::ReadOnly);
    file_button_8.open          (QFile::ReadOnly);
    stylesheet_button_flag      = QLatin1String(file_button_flag.readAll());
    stylesheet_button_mine      = QLatin1String(file_button_mine.readAll());
    stylesheet_button_mine_hit  = QLatin1String(file_button_mine_hit.readAll());
    stylesheet_button_covered   = QLatin1String(file_button_covered.readAll());
    stylesheet_button_uncovered = QLatin1String(file_button_uncovered.readAll());
    stylesheet_button_1         = QLatin1String(file_button_1.readAll());
    stylesheet_button_2         = QLatin1String(file_button_2.readAll());
    stylesheet_button_3         = QLatin1String(file_button_3.readAll());
    stylesheet_button_4         = QLatin1String(file_button_4.readAll());
    stylesheet_button_5         = QLatin1String(file_button_5.readAll());
    stylesheet_button_6         = QLatin1String(file_button_6.readAll());
    stylesheet_button_7         = QLatin1String(file_button_7.readAll());
    stylesheet_button_8         = QLatin1String(file_button_8.readAll());
    file_button_flag.close();
    file_button_mine.close();
    file_button_mine_hit.close();
    file_button_covered.close();
    file_button_uncovered.close();
    file_button_1.close();
    file_button_2.close();
    file_button_3.close();
    file_button_4.close();
    file_button_5.close();
    file_button_6.close();
    file_button_7.close();
    file_button_8.close();
}

Field::~Field()
{
    for (int i=0; i <= cols; i++)
        delete[] this->cell[i];
    delete[] this->cell;
}

QVector<QVector<char>> Field::createArray()
{
    QVector<QVector<char>> temp2DVector;
    for(int i = 0; i <= this->cols; ++i)
    {
        QVector<char> row;
        for(int j = 0; j <= this->rows; ++j)
        {
            row.push_back(' ');
        }
        temp2DVector.push_back(row);
    }
    return temp2DVector;
}

void Field::createCell()
{
    this->cell = new Cell*[this->cols + 1];
    for (int i=0; i < (this->cols + 1); i++)
        this->cell[i] = new Cell[this->rows + 1];
}

// place mines at random positions of this->minesArray[][]:
void Field::fillMinesArray(const Common::Coords& userFirstInput)
{
    Common::Coords coords;
    int sizeOfFieldArray = this->cols * this->rows;
    QVector<int> tempVector;
    for (int i = 1; i <= sizeOfFieldArray; i++)
    {
        if (i != Common::structToInt(userFirstInput, this->cols))
            tempVector.push_back(i);
    }

    std::random_shuffle(tempVector.begin(), tempVector.end());
    for (int i = 0; i < this->mines; i++)
    {
        coords = Common::intToStruct(tempVector.at(i), this->cols);
        this->minesArray[coords.col][coords.row] = 'X';
    }
}

void Field::addCells()
{
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            this->cell[i][j].setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            this->cell[i][j].setStyleSheet(stylesheet_button_covered);
            //this->cell[i][j].setFixedSize(this->cellSize, this->cellSize);
            layout->addWidget(&this->cell[i][j], j - 1, i - 1, 1, 1);
            connect(&this->cell[i][j], SIGNAL(doubleClicked()), this, SLOT(onDoubleClicked()));
            connect(&this->cell[i][j], SIGNAL(leftReleased()), this, SLOT(onLeftReleased()));
            connect(&this->cell[i][j], SIGNAL(rightReleased()), this, SLOT(onRightReleased()));
        }
    }
}

// return the coords of a button in the QGridLayout for use in an array[][]:
Common::Coords Field::gridPosition(Cell* button)
{
  Common::Coords coords;
  if (! button->parentWidget()) return coords;
  auto layout = qobject_cast<QGridLayout*>(button->parentWidget()->layout());
  if (! layout) return coords;
  int index = layout->indexOf(button);
  Q_ASSERT(index >= 0);
  int _ = 0;
  layout->getItemPosition(index, &coords.row, &coords.col, &_, &_);
  coords.col++;
  coords.row++;
  return coords;
}

// test coords if they contain a flag:
bool Field::isFlagSet(const Common::Coords& coords)
{
    if (this->fieldArray[coords.col][coords.row] == 'F')
        return true;
    else
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(const Common::Coords& coords)
{
    for (int i = 1; i < 8; i++)
    {
        if (this->fieldArray[coords.col][coords.row] == i + 48)
            return true;
    }
    return false;
}

// find neighbours of a cell at "coords" that hold a given content (passed by variable content)
QVector<Common::Coords> Field::findNeighbours(const QVector<QVector<char>>& tempArray, const Common::Coords& coords, const char& content)
{
    QVector<Common::Coords> findNeighboursReturn;
    std::array<std::array<int, 2>, 8> neighboursArray;
    neighboursArray.at(0) = { -1, -1 };
    neighboursArray.at(1) = {  0, -1 };
    neighboursArray.at(2) = {  1, -1 };
    neighboursArray.at(3) = {  1,  0 };
    neighboursArray.at(4) = {  1,  1 };
    neighboursArray.at(5) = {  0,  1 };
    neighboursArray.at(6) = { -1,  1 };
    neighboursArray.at(7) = { -1,  0 };

    for (size_t x = 0; x < sizeof(neighboursArray) / sizeof(neighboursArray[0]); ++x)
    {
        if (
            !(coords.col == 1 && neighboursArray[x][0] == -1) &&
            !(coords.row == 1 && neighboursArray[x][1] == -1) &&
            !(coords.col == this->cols && neighboursArray[x][0] == 1) &&
            !(coords.row == this->rows && neighboursArray[x][1] == 1)
            )
        {
            if (content == isNumber(coords))
            {
                for (int i = 1; i < 8; ++i)
                {
                    if (tempArray[coords.col + neighboursArray[x][0]][coords.row + neighboursArray[x][1]] == i)
                    {
                        Common::Coords tempCoords;
                        tempCoords.col = coords.col + neighboursArray[x][0];
                        tempCoords.row = coords.row + neighboursArray[x][1];
                        findNeighboursReturn.push_back(tempCoords);
                    }
                }
            }
            else if (tempArray[coords.col + neighboursArray[x][0]][coords.row + neighboursArray[x][1]] == content)
            {
                Common::Coords tempCoords;
                tempCoords.col = coords.col + neighboursArray[x][0];
                tempCoords.row = coords.row + neighboursArray[x][1];
                findNeighboursReturn.push_back(tempCoords);
            }
        }
    }
    return findNeighboursReturn;
}

// print the number of surrounding mines in this->cell[coords.col][coords.row]:
void Field::printNumber(const Common::Coords& coords, const int& number)
{
    if (number == 0)
    {
        this->fieldArray[coords.col][coords.row] = '0';
        this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_uncovered);
    }
    else
    {
        if (number == 1)
        {
            this->fieldArray[coords.col][coords.row] = '1';
            this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_1);
        }
        if (number == 2)
        {
            this->fieldArray[coords.col][coords.row] = '2';
            this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_2);
        }
        if (number == 3)
        {
            this->fieldArray[coords.col][coords.row] = '3';
            this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_3);
        }
        if (number == 4)
        {
            this->fieldArray[coords.col][coords.row] = '4';
            this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_4);
        }
        if (number == 5)
        {
            this->fieldArray[coords.col][coords.row] = '5';
            this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_5);
        }
        if (number == 6)
        {
            this->fieldArray[coords.col][coords.row] = '6';
            this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_6);
        }
        if (number == 7)
        {
            this->fieldArray[coords.col][coords.row] = '7';
            this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_7);
        }
        if (number == 8)
        {
            this->fieldArray[coords.col][coords.row] = '8';
            this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_8);
        }
    }
}

// used for both winning and losing a game (mode = "win" or "lose"):
void Field::gameOver(const Common::Coords& coords, const QString& mode)
{
    this->gameover = true;

    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = i;
            coordsTemp.row = j;
            QVector<Common::Coords> neighboursMinesVector;
            neighboursMinesVector = findNeighbours(this->minesArray, coordsTemp, 'X');
            printNumber(coordsTemp, static_cast<int>(neighboursMinesVector.size()));
            if (this->minesArray[i][j] == 'X')
            {
                this->cell[i][j].setStyleSheet(stylesheet_button_mine);
            }
            else if (this->minesArray[i][j] == 'H')
            {
                this->cell[i][j].setStyleSheet(stylesheet_button_mine_hit);
            }
        }
    }
    if (mode == "lose")
        this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_mine_hit);
}

// automatically uncover all connected cells, as long as they have no neighbour mines:
void Field::autoReveal(const Common::Coords& coords, QVector<int>& poolVector)
{
    // create vector holding covered neighbours:
    QVector<Common::Coords> neighboursCoveredVector;
    neighboursCoveredVector = findNeighbours(this->fieldArray, coords, ' ');

    for (int i = 0; i < neighboursCoveredVector.size(); ++i)
    {
        QVector<Common::Coords> neighboursMinesVector;
        neighboursMinesVector = findNeighbours(this->minesArray, neighboursCoveredVector.at(i), 'X');
        if (std::find(poolVector.begin(), poolVector.end(), Common::structToInt(neighboursCoveredVector.at(i), this->cols)) == poolVector.end())
        {
            if (this->minesArray[neighboursCoveredVector.at(i).col][neighboursCoveredVector.at(i).row] != 'X')
            {
                QVector<Common::Coords> neighboursUncoveredVector;
                neighboursUncoveredVector = findNeighbours(this->fieldArray, neighboursCoveredVector.at(i), '0');
                if (neighboursUncoveredVector.size() == 0)
                {
                    continue;
                }
                else
                {
                    poolVector.push_back(Common::structToInt(neighboursCoveredVector.at(i), this->cols));
                    printNumber(neighboursCoveredVector.at(i), neighboursMinesVector.size());
                    --this->countCovered;
                }
            }
        }
        if (neighboursMinesVector.size() == 0)
        {
            autoReveal(neighboursCoveredVector.at(i), poolVector);
        }
    }
}

void Field::flagAutoUncover(const Common::Coords& coords)
{
    // create a new vector of surrounding flags:
    QVector<Common::Coords> flagUncoverNeighboursFlagsVector;
    flagUncoverNeighboursFlagsVector = findNeighbours(this->fieldArray, coords, 'F');

    // if player has placed some flags around UserInput.coords:
    if (flagUncoverNeighboursFlagsVector.size() != 0)
    {
        // create a new vector of surrounding mines:
        QVector<Common::Coords> flagUncoverNeighboursMinesVector;
        flagUncoverNeighboursMinesVector = findNeighbours(this->minesArray, coords, 'X');

        // only proceed if the flag number matches the number of actual surrounding mines:
        if (flagUncoverNeighboursMinesVector.size() == flagUncoverNeighboursFlagsVector.size())
        {
            // create a new vector of surrounding covered squares:
            QVector<Common::Coords> flagUncoverNeighboursCoveredVector;
            flagUncoverNeighboursCoveredVector = findNeighbours(this->fieldArray, coords, ' ');

            // create a new empty vector for missed mines:
            QVector<Common::Coords> flagUncoverMissedMinesVector;

            // for each covered neighbour of userInput.coords check if the player has missed a mine
            // and add this mines position to flagUncoverMissedMinesVector:
            for (int i = 0; i < flagUncoverNeighboursCoveredVector.size(); ++i)
            {
                if (this->minesArray[flagUncoverNeighboursCoveredVector.at(i).col][flagUncoverNeighboursCoveredVector.at(i).row] == 'X')
                {
                    flagUncoverMissedMinesVector.push_back(flagUncoverNeighboursCoveredVector.at(i));
                }
            }
            // if there are missed mines, reveal the minesArray - player has lost:
            if (flagUncoverMissedMinesVector.size() != 0)
            {
                for (int i = 0; i < flagUncoverMissedMinesVector.size(); ++i)
                {
                    this->minesArray[flagUncoverMissedMinesVector.at(i).col][flagUncoverMissedMinesVector.at(i).row] = 'H';
                }
                this->gameover = true;
                Common::Coords dummyCoords;
                gameOver(dummyCoords, "lost");
            }
            // else if all flags are placed correctly:
            else
            {
                if (flagUncoverNeighboursMinesVector.size() == flagUncoverNeighboursFlagsVector.size())
                {
                    // create a pool of already uncovered cells, to avoid double checks within autoUncoverRecursive():
                    QVector<int> poolVector;
                    // for each covered neighbour of userInput.coords, print the number of surrounding mines:
                    for (int i = 0; i < flagUncoverNeighboursCoveredVector.size(); ++i)
                    {
                        Common::Coords tempCoords;
                        tempCoords.col = flagUncoverNeighboursCoveredVector.at(i).col;
                        tempCoords.row = flagUncoverNeighboursCoveredVector.at(i).row;
                        QVector<Common::Coords> flagUncoverNeighboursCoveredMinesVector;
                        flagUncoverNeighboursCoveredMinesVector = findNeighbours(this->minesArray, tempCoords, 'X');

                        if (flagUncoverNeighboursCoveredMinesVector.size() == 0)
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), Common::structToInt(tempCoords, this->cols)) == poolVector.end())
                            {
                                printNumber(tempCoords, 0);
                                poolVector.push_back(Common::structToInt(tempCoords, this->cols));
                                --this->countCovered;
                                autoReveal(tempCoords, poolVector);
                            }
                        }
                        else
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), Common::structToInt(tempCoords, this->cols)) == poolVector.end())
                            {
                                printNumber(tempCoords, static_cast<int>(flagUncoverNeighboursCoveredMinesVector.size()));
                                poolVector.push_back(Common::structToInt(tempCoords, this->cols));
                                --this->countCovered;
                            }
                        }
                    }
                }
            }
        }
    }
}


// handle left clicking on a cell:
void Field::onLeftReleased()
{
    if (this->gameover != true)
    {
        Cell *button = qobject_cast<Cell*>(sender());
        Common::Coords coordsTemp = gridPosition(button);

        if (fieldArray[coordsTemp.col][coordsTemp.row] != 'F' && (! isNumber(coordsTemp)))
        {
            QVector<Common::Coords> neighboursMinesVector;

            // fill minesArray with mines only once after users first guess:
            if (this->firstTurn)
                fillMinesArray(coordsTemp);

            // if user hit a mine, reveal the game field - game is lost:
            if (this->minesArray[coordsTemp.col][coordsTemp.row] == 'X')
            {
                gameOver(coordsTemp, "lose");
            }

            else
            {
                // uncover the players choice and place the number of surrounding mines in it:
                neighboursMinesVector = findNeighbours(this->minesArray, coordsTemp, 'X');
                printNumber(coordsTemp, neighboursMinesVector.size());
                --this->countCovered;
            }

            // automatically uncover all neighbours of squares with no neighbour mines:
            QVector<int> poolVector;
            autoReveal(coordsTemp, poolVector);
            this->firstTurn = false;
        }
    }

    // check if player has won:
    if (this->flagsCount + this->countCovered == this->mines)
    {
        this->gameover = true;
        for (int i = 1; i <= this->cols; i++)
        {
            for (int j = 1; j <= this->rows; j++)
            {
                if (this->minesArray[i][j] == 'X')
                    this->cell[i][j].setStyleSheet(stylesheet_button_mine);
                else
                    this->cell[i][j].setStyleSheet(stylesheet_button_uncovered);
            }
        }
        Common::Coords dummyCoords;
        dummyCoords.col = 1;
        dummyCoords.row = 1;
        gameOver(dummyCoords, "win");
    }
}

// place and remove flags with right click:
void Field::onRightReleased()
{
    if (this->gameover != true)
    {
        Cell *button = qobject_cast<Cell *>(sender());
        auto coords = gridPosition(button);
        if (this->fieldArray[coords.col][coords.row] == ' ' || this->fieldArray[coords.col][coords.row] == 'F')
        {
            if (! isFlagSet(coords))
            {
                button->setStyleSheet(stylesheet_button_flag);
                this->fieldArray[coords.col][coords.row] = 'F';
                this->flagsCount++;
                this->minesLeft--;
                this->countCovered--;
            }
            else
            {
                button->setStyleSheet(stylesheet_button_covered);
                this->fieldArray[coords.col][coords.row] = ' ';
                this->flagsCount--;
                this->minesLeft++;
                this->countCovered++;
            }
        }
    }
}

// auto-reveal safe cells with no neighbour mines by double clicking on a number
// (flags around the numbered button must be placed right):
void Field::onDoubleClicked()
{
    if (this->gameover != true)
    {
        Cell *button = qobject_cast<Cell *>(sender());
        Common::Coords coords = gridPosition(button);
        if (isNumber(coords))
        {
            flagAutoUncover(coords);
        }
    }
}
