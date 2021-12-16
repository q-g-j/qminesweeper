#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSizePolicy>
#include <QGridLayout>

#include <QVector>

#include "cell.h"
#include "common.h"
#include "field.h"

Field::Field(QWidget *parent, const int& cols_, const int& rows_, const int& mines_, const int& cellSize_, QLabel *labelMinesLeft_, QPushButton *smiley_, Timer *timer_) : QWidget(parent)
{
    this->cols = cols_;
    this->rows = rows_;
    this->mines = mines_;
    this->cellSize = cellSize_;
    this->minesLeft = mines_;
    this->labelMinesLeft = labelMinesLeft_;
    this->smiley = smiley_;
    this->timer = timer_;
    this->countUnrevealed = cols_ * rows_;
    this->flagsCount = 0;
    this->firstTurn = true;
    this->gameover = false;
    this->fieldArray = createArray();
    this->minesArray = createArray();
    this->labelMinesLeft->setText(QString::number(this->minesLeft));
    createCells();

    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    QFile file_button_flag          (":/stylesheet/button_flag.css");
    QFile file_button_mine          (":/stylesheet/button_mine.css");
    QFile file_button_mine_hit      (":/stylesheet/button_mine_hit.css");
    QFile file_button_revealed      (":/stylesheet/button_revealed.css");
    QFile file_button_unrevealed    (":/stylesheet/button_unrevealed.css");
    QFile file_button_1             (":/stylesheet/button_1.css");
    QFile file_button_2             (":/stylesheet/button_2.css");
    QFile file_button_3             (":/stylesheet/button_3.css");
    QFile file_button_4             (":/stylesheet/button_4.css");
    QFile file_button_5             (":/stylesheet/button_5.css");
    QFile file_button_6             (":/stylesheet/button_6.css");
    QFile file_button_7             (":/stylesheet/button_7.css");
    QFile file_button_8             (":/stylesheet/button_8.css");
    QFile smiley                    (":/stylesheet/infobar_smiley.css");
    QFile smiley_pressed            (":/stylesheet/infobar_smiley_pressed.css");
    QFile smiley_won                (":/stylesheet/infobar_smiley_won.css");
    QFile smiley_lost               (":/stylesheet/infobar_smiley_lost.css");
    file_button_flag.open           (QFile::ReadOnly);
    file_button_mine.open           (QFile::ReadOnly);
    file_button_mine_hit.open       (QFile::ReadOnly);
    file_button_revealed.open       (QFile::ReadOnly);
    file_button_unrevealed.open     (QFile::ReadOnly);
    file_button_1.open              (QFile::ReadOnly);
    file_button_2.open              (QFile::ReadOnly);
    file_button_3.open              (QFile::ReadOnly);
    file_button_4.open              (QFile::ReadOnly);
    file_button_5.open              (QFile::ReadOnly);
    file_button_6.open              (QFile::ReadOnly);
    file_button_7.open              (QFile::ReadOnly);
    file_button_8.open              (QFile::ReadOnly);
    smiley.open                     (QFile::ReadOnly);
    smiley_pressed.open             (QFile::ReadOnly);
    smiley_won.open                 (QFile::ReadOnly);
    smiley_lost.open                (QFile::ReadOnly);
    stylesheet_button_flag          = QLatin1String(file_button_flag.readAll());
    stylesheet_button_mine          = QLatin1String(file_button_mine.readAll());
    stylesheet_button_mine_hit      = QLatin1String(file_button_mine_hit.readAll());
    stylesheet_button_revealed      = QLatin1String(file_button_revealed.readAll());
    stylesheet_button_unrevealed    = QLatin1String(file_button_unrevealed.readAll());
    stylesheet_button_1             = QLatin1String(file_button_1.readAll());
    stylesheet_button_2             = QLatin1String(file_button_2.readAll());
    stylesheet_button_3             = QLatin1String(file_button_3.readAll());
    stylesheet_button_4             = QLatin1String(file_button_4.readAll());
    stylesheet_button_5             = QLatin1String(file_button_5.readAll());
    stylesheet_button_6             = QLatin1String(file_button_6.readAll());
    stylesheet_button_7             = QLatin1String(file_button_7.readAll());
    stylesheet_button_8             = QLatin1String(file_button_8.readAll());
    stylesheet_smiley               = QLatin1String(smiley.readAll());
    stylesheet_smiley_pressed       = QLatin1String(smiley_pressed.readAll());
    stylesheet_smiley_won           = QLatin1String(smiley_won.readAll());
    stylesheet_smiley_lost          = QLatin1String(smiley_lost.readAll());
    file_button_flag.close();
    file_button_mine.close();
    file_button_mine_hit.close();
    file_button_revealed.close();
    file_button_unrevealed.close();
    file_button_1.close();
    file_button_2.close();
    file_button_3.close();
    file_button_4.close();
    file_button_5.close();
    file_button_6.close();
    file_button_7.close();
    file_button_8.close();
    smiley.close();
    smiley_pressed.close();
    smiley_won.close();
    smiley_lost.close();
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

void Field::createCells()
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
        if (i != Common::CoordsToInt(userFirstInput, this->cols))
            tempVector.push_back(i);
    }

    std::random_shuffle(tempVector.begin(), tempVector.end());
    for (int i = 0; i < this->mines; i++)
    {
        coords = Common::intToCoords(tempVector.at(i), this->cols);
        this->minesArray[coords.col][coords.row] = 'X';
    }
}

void Field::addCells()
{
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            this->cell[i][j].setStyleSheet(stylesheet_button_unrevealed);
            this->cell[i][j].setFixedSize(this->cellSize, this->cellSize);
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
        this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_revealed);
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
            if (this->minesArray[i][j] == 'X')
                this->cell[i][j].setStyleSheet(stylesheet_button_mine);
            else
                this->cell[i][j].setStyleSheet(stylesheet_button_revealed);

            if (this->minesArray[i][j] != 'X' && this->minesArray[i][j] != 'H')
            {
                Common::Coords coordsTemp;
                coordsTemp.col = i;
                coordsTemp.row = j;
                QVector<Common::Coords> neighboursMinesVector = findNeighbours(this->minesArray, coordsTemp, 'X');
                QVector<Common::Coords> neighboursMinesHitVector = findNeighbours(this->minesArray, coordsTemp, 'H');
                printNumber(coordsTemp, static_cast<int>(neighboursMinesVector.size() + neighboursMinesHitVector.size()));
            }
            else if (this->minesArray[i][j] == 'X')
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
    {
        this->cell[coords.col][coords.row].setStyleSheet(stylesheet_button_mine_hit);
        this->smiley->setStyleSheet(stylesheet_smiley_lost);
    }
    else
        this->smiley->setStyleSheet(stylesheet_smiley_won);
    this->minesLeft = 0;
    this->labelMinesLeft->setText(QString::number(this->minesLeft));
    timer->keepGoing = false;
}

// automatically reveal all connected cells, as long as they have no neighbour mines:
void Field::autoReveal(const Common::Coords& coords, QVector<int>& poolVector)
{
    // create vector holding unrevealed neighbours:
    QVector<Common::Coords> neighboursUnrevealedVector;
    neighboursUnrevealedVector = findNeighbours(this->fieldArray, coords, ' ');

    for (int i = 0; i < neighboursUnrevealedVector.size(); ++i)
    {
        QVector<Common::Coords> neighboursMinesVector;
        neighboursMinesVector = findNeighbours(this->minesArray, neighboursUnrevealedVector.at(i), 'X');
        if (std::find(poolVector.begin(), poolVector.end(), Common::CoordsToInt(neighboursUnrevealedVector.at(i), this->cols)) == poolVector.end())
        {
            if (this->minesArray[neighboursUnrevealedVector.at(i).col][neighboursUnrevealedVector.at(i).row] != 'X')
            {
                QVector<Common::Coords> neighboursZerosVector;
                neighboursZerosVector = findNeighbours(this->fieldArray, neighboursUnrevealedVector.at(i), '0');
                if (neighboursZerosVector.size() == 0)
                {
                    continue;
                }
                else
                {
                    poolVector.push_back(Common::CoordsToInt(neighboursUnrevealedVector.at(i), this->cols));
                    printNumber(neighboursUnrevealedVector.at(i), neighboursMinesVector.size());
                    --this->countUnrevealed;
                }
            }
        }
        if (neighboursMinesVector.size() == 0)
        {
            autoReveal(neighboursUnrevealedVector.at(i), poolVector);
        }
    }
}

void Field::flagAutoReveal(const Common::Coords& coords)
{
    // create a new vector of surrounding flags:
    QVector<Common::Coords> flagRevealNeighboursFlagsVector;
    flagRevealNeighboursFlagsVector = findNeighbours(this->fieldArray, coords, 'F');

    // if player has placed some flags around UserInput.coords:
    if (flagRevealNeighboursFlagsVector.size() != 0)
    {
        // create a new vector of surrounding mines:
        QVector<Common::Coords> flagRevealNeighboursMinesVector;
        flagRevealNeighboursMinesVector = findNeighbours(this->minesArray, coords, 'X');

        // only proceed if the flag number matches the number of actual surrounding mines:
        if (flagRevealNeighboursMinesVector.size() == flagRevealNeighboursFlagsVector.size())
        {
            // create a new vector of surrounding unrevealed squares:
            QVector<Common::Coords> flagRevealNeighboursUnrevealedVector;
            flagRevealNeighboursUnrevealedVector = findNeighbours(this->fieldArray, coords, ' ');

            // create a new empty vector for missed mines:
            QVector<Common::Coords> flagRevealMissedMinesVector;

            // for each unrevealed neighbour of userInput.coords check if the player has missed a mine
            // and add this mines position to flagRevealMissedMinesVector:
            for (int i = 0; i < flagRevealNeighboursUnrevealedVector.size(); ++i)
            {
                if (this->minesArray[flagRevealNeighboursUnrevealedVector.at(i).col][flagRevealNeighboursUnrevealedVector.at(i).row] == 'X')
                {
                    flagRevealMissedMinesVector.push_back(flagRevealNeighboursUnrevealedVector.at(i));
                }
            }
            // if there are missed mines, reveal the minesArray - player has lost:
            if (flagRevealMissedMinesVector.size() != 0)
            {
                for (int i = 0; i < flagRevealMissedMinesVector.size(); ++i)
                {
                    this->minesArray[flagRevealMissedMinesVector.at(i).col][flagRevealMissedMinesVector.at(i).row] = 'H';
                }
                Common::Coords dummyCoords;
                gameOver(dummyCoords, "lose");
            }
            // else if all flags are placed correctly:
            else
            {
                if (flagRevealNeighboursMinesVector.size() == flagRevealNeighboursFlagsVector.size())
                {
                    // create a pool of already revealed cells, to avoid double checks within autoRevealRecursive():
                    QVector<int> poolVector;
                    // for each unrevealed neighbour of userInput.coords, print the number of surrounding mines:
                    for (int i = 0; i < flagRevealNeighboursUnrevealedVector.size(); ++i)
                    {
                        Common::Coords tempCoords;
                        tempCoords.col = flagRevealNeighboursUnrevealedVector.at(i).col;
                        tempCoords.row = flagRevealNeighboursUnrevealedVector.at(i).row;
                        QVector<Common::Coords> flagRevealNeighboursUnrevealedMinesVector;
                        flagRevealNeighboursUnrevealedMinesVector = findNeighbours(this->minesArray, tempCoords, 'X');

                        if (flagRevealNeighboursUnrevealedMinesVector.size() == 0)
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), Common::CoordsToInt(tempCoords, this->cols)) == poolVector.end())
                            {
                                printNumber(tempCoords, 0);
                                poolVector.push_back(Common::CoordsToInt(tempCoords, this->cols));
                                --this->countUnrevealed;
                                autoReveal(tempCoords, poolVector);
                            }
                        }
                        else
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), Common::CoordsToInt(tempCoords, this->cols)) == poolVector.end())
                            {
                                printNumber(tempCoords, static_cast<int>(flagRevealNeighboursUnrevealedMinesVector.size()));
                                poolVector.push_back(Common::CoordsToInt(tempCoords, this->cols));
                                --this->countUnrevealed;
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
                // reveal the players choice and place the number of surrounding mines in it:
                neighboursMinesVector = findNeighbours(this->minesArray, coordsTemp, 'X');
                printNumber(coordsTemp, neighboursMinesVector.size());
                --this->countUnrevealed;
            }

            // automatically reveal all neighbours of squares with no neighbour mines:
            QVector<int> poolVector;
            autoReveal(coordsTemp, poolVector);
            this->firstTurn = false;
        }
    }

    // check if player has won:
    if (this->flagsCount + this->countUnrevealed == this->mines)
    {
        Common::Coords dummyCoords;
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
                this->countUnrevealed--;
            }
            else
            {
                button->setStyleSheet(stylesheet_button_unrevealed);
                this->fieldArray[coords.col][coords.row] = ' ';
                this->flagsCount--;
                this->minesLeft++;
                this->countUnrevealed++;
            }
        }
    }
    this->labelMinesLeft->setText(QString::number(this->minesLeft));
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
            flagAutoReveal(coords);
        }
    }
    // check if player has won:
    if (this->flagsCount + this->countUnrevealed == this->mines)
    {
        Common::Coords dummyCoords;
        dummyCoords.col = 1;
        dummyCoords.row = 1;
        gameOver(dummyCoords, "win");
    }
}
