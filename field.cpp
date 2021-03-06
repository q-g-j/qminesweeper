#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSizePolicy>
#include <QGridLayout>

#include <vector>

#include "cell.h"
#include "common.h"
#include "field.h"
#include "mainwindow.h"

Field::Field(QWidget *parent, int const& cols, int const& rows, int const& mines, int const& cellSize) : QWidget(parent)
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
    this->flagsCount = 0;
    this->cell = new Cell*[this->cols + 1];
    for (int i=0; i < (this->cols + 1); i++)
        this->cell[i] = new Cell[this->rows + 1];

    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
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

    for (int i=0; i <= cols; i++)
        delete[] this->fieldArray[i];
    delete[] this->fieldArray;

    for (int i=0; i <= cols; i++)
        delete[] this->minesArray[i];
    delete[] this->minesArray;
}

char** Field::createArray()
{
    const int colsNum = this->cols + 1;
    const int rowsNum = this->rows + 1;
    char** tempArray = nullptr;
    tempArray = new char*[colsNum];
    for (int i=0; i < colsNum; i++)
        tempArray[i] = new char[rowsNum];

    for (int i = 0; i < colsNum; i++)
        for (int j = 0; j < rowsNum; j++)
            tempArray[i][j] = ' ';
    return tempArray;
}

// place mines at random positions of this->minesArray[][]:
void Field::fillMinesArray(Common::Coords& userFirstInput)
{
    Common::Coords coords;
    int sizeOfFieldArray = this->cols * this->rows;
    std::vector<int> tempVector;
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
Common::Coords Field::gridPosition(Cell * button)
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
bool Field::isFlagSet(Common::Coords& coords)
{
    if (this->fieldArray[coords.col][coords.row] == 'F')
        return true;
    else
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(Common::Coords& coords)
{
    for (int i = 1; i < 8; i++)
    {
        if (this->fieldArray[coords.col][coords.row] == i + 48)
            return true;
    }
    return false;
}

// find neighbors of a cell at "coords" that hold a given content (passed by variable symbol)
std::vector<Common::Coords> Field::findNeighbours(char** tempArray, Common::Coords const& coords, char const& content)
{
    std::vector<Common::Coords> neighboursVector;

    // up left:
    if (coords.col-1 > 0 && coords.row-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row-1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col-1;
            coordsTemp.row = coords.row-1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // up middle:
    if (coords.row-1 > 0)
    {
        if (tempArray[coords.col][coords.row-1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col;
            coordsTemp.row = coords.row-1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // up right:
    if (coords.col+1 <= this->cols && coords.row-1 > 0)
    {
        if (tempArray[coords.col+1][coords.row-1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row-1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // middle left:
    if (coords.col-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col-1;
            coordsTemp.row = coords.row;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // middle right:
    if (coords.col+1 <= this->cols)
    {
        if (tempArray[coords.col+1][coords.row] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // down left:
    if (coords.col-1 > 0 && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col-1][coords.row+1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col-1;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // down middle:
    if (coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col][coords.row+1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // down right:
    if (coords.col+1 <= this->cols && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col+1][coords.row+1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    return neighboursVector;
}

// print the number of surrounding mines in this->cell[coords.col][coords.row]:
void Field::printNumber(const Common::Coords &coords, const int &number)
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
void Field::gameOver(Common::Coords const& coords, QString const& mode)
{
    this->gameover = true;

    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = i;
            coordsTemp.row = j;
            std::vector<Common::Coords> neighboursMinesVector;
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

// automatically uncover all neighbours of squares with no neighbor mines:
void Field::autoReveal(std::vector<Common::Coords>& neighboursMinesVector)
{
    bool run = true;
    if (neighboursMinesVector.size() == 0)
    {
        while (run == true)
        {
            // for each free position do:
            for (int i = 1; i <= this->cols; i++)
            {
                for (int j = 1; j <= this->rows; j++)
                {
                    if (this->fieldArray[i][j] == ' ')
                    {
                        // create a new vector of neighbours containing a '0':
                        Common::Coords coordsBase;
                        coordsBase.col = i;
                        coordsBase.row = j;
                        std::vector<Common::Coords> neighboursZerosVector;
                        neighboursZerosVector = findNeighbours(this->fieldArray, coordsBase, '0');


                        // if there is a neighbour containing a '0' create a new vector of neighbours containing mines:
                        if (neighboursZerosVector.size() != 0)
                        {
                            std::vector<Common::Coords> neighboursMinesVectorNew;
                            neighboursMinesVectorNew = findNeighbours(this->minesArray, coordsBase, 'X');

                            // place neighboursMinesVectorNew.size() in fieldArray:
                            Common::Coords coordsTemp;
                            coordsTemp.col = i;
                            coordsTemp.row = j;
                            printNumber(coordsTemp, static_cast<int>(neighboursMinesVectorNew.size()));
                            this->countCovered--;
                        }
                    }
                }
            }
            run = false;

            // repeat if necessary:
            for (int a = 1; a <= this->cols; a++)
            {
                for (int b = 1; b <= this->rows; b++)
                {
                    if (this->fieldArray[a][b] == ' ')
                    {
                        // create a new vector of neighbours containing '0':
                        Common::Coords coordsBaseNew;
                        coordsBaseNew.col = a;
                        coordsBaseNew.row = b;
                        std::vector<Common::Coords> neighboursZerosVectorNew;
                        neighboursZerosVectorNew = findNeighbours(this->fieldArray, coordsBaseNew, '0');
                        if (neighboursZerosVectorNew.size() != 0)
                            run = true;
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
        Cell *button = qobject_cast<Cell *>(sender());
        auto coordsTemp = gridPosition(button);

        if (fieldArray[coordsTemp.col][coordsTemp.row] != 'F' && (! isNumber(coordsTemp)))
        {
            std::vector<Common::Coords> neighboursMinesVector;

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
                printNumber(coordsTemp, static_cast<int>(neighboursMinesVector.size()));
                this->countCovered--;
            }

            // automatically uncover all neighbours of squares with no neighbor mines:
            autoReveal(neighboursMinesVector);
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
        auto coords = gridPosition(button);
        if (isNumber(coords))
        {
            // create a new vector of surrounding mines:
            std::vector<Common::Coords> autoUncoverNeighboursMinesVector;
            autoUncoverNeighboursMinesVector = findNeighbours(this->minesArray, coords, 'X');

            // create a new vector of surrounding flags:
            std::vector<Common::Coords> autoUncoverNeighboursFlagsVector;
            autoUncoverNeighboursFlagsVector = findNeighbours(this->fieldArray, coords, 'F');

            // create a new vector of surrounding covered squares:
            std::vector<Common::Coords> autoUncoverNeighboursCoveredVector;
            autoUncoverNeighboursCoveredVector = findNeighbours(this->fieldArray, coords, ' ');

            // create a new empty vector for missed mines:
            std::vector<Common::Coords> autoUncoverMissedMinesVector;

            // if player has placed some flags around coords:
            if (autoUncoverNeighboursFlagsVector.size() != 0)
            {
                // only proceed if the flag number matches the number of actual surrounding mines:
                if (autoUncoverNeighboursMinesVector.size() == autoUncoverNeighboursFlagsVector.size())
                {
                    // for each not uncovered neighbour of clicked button check if the player has missed a mine
                    // and add this mines position to autoUncoverMissedMinesVector:
                    for (int i = 0; i < static_cast<int>(autoUncoverNeighboursCoveredVector.size()); i++)
                    {
                        if (this->minesArray[autoUncoverNeighboursCoveredVector.at(i).col][autoUncoverNeighboursCoveredVector.at(i).row] == 'X')
                            autoUncoverMissedMinesVector.push_back(autoUncoverNeighboursCoveredVector.at(i));
                    }
                    // if there are missed mines, reveal the minesArray - player has lost:
                    if (autoUncoverMissedMinesVector.size() != 0)
                    {                        
                        for (int i = 0; i < static_cast<int>(autoUncoverMissedMinesVector.size()); i++)
                        {
                            this->minesArray[autoUncoverMissedMinesVector.at(i).col][autoUncoverMissedMinesVector.at(i).row] = 'H';
                        }
                        // call gameOver() with dummy coords:
                        Common::Coords dummyCoords;
                        dummyCoords.col = autoUncoverMissedMinesVector.at(0).col;
                        dummyCoords.row = autoUncoverMissedMinesVector.at(0).row;
                        gameOver(dummyCoords, "lose");
                    }

                    // else if all flags are placed correctly:
                    else
                    {
                        std::vector<Common::Coords> autoUncoverNeighboursCoveredMinesVector;
                        if (autoUncoverNeighboursMinesVector.size() == autoUncoverNeighboursFlagsVector.size())
                        {
                            // for each not uncovered neighbour of coords, print the number of surrounding mines:
                            for (int i = 0; i < static_cast<int>(autoUncoverNeighboursCoveredVector.size()); i++)
                            {
                                Common::Coords coordsTemp;
                                coordsTemp.col = autoUncoverNeighboursCoveredVector.at(i).col;
                                coordsTemp.row = autoUncoverNeighboursCoveredVector.at(i).row;
                                autoUncoverNeighboursCoveredMinesVector = findNeighbours(this->minesArray, coordsTemp, 'X');
                                printNumber(coordsTemp, static_cast<int>(autoUncoverNeighboursCoveredMinesVector.size()));
                                this->countCovered--;
                            }
                        }

                        // automatically uncover all neighbours of squares with no neighbor mines:
                        autoReveal(autoUncoverNeighboursCoveredMinesVector);
                    }
                }
            }
        }
    }
}
