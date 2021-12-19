#include <QDebug>
#include <QFile>
#include <QSizePolicy>
#include <QVector>

#include "cell.h"
#include "common.h"
#include "field.h"

Field::Field(QWidget *parent, const int& cols_, const int& rows_, const int& mines_, const int& cellSize_) : QWidget(parent)
{
    this->cols = cols_;
    this->rows = rows_;
    this->mines = mines_;
    this->cellSize = cellSize_;
    this->minesLeft = mines_;
    this->countUnrevealed = cols_ * rows_;
    this->flagsCount = 0;
    this->firstTurn = true;
    this->isGameOver = false;
    this->field2DVector = this->create2DVector();
    this->mines2DVector = this->create2DVector();
    emit this->minesleft_changed(this->minesLeft);
    this->createCells();

    this->layout = new QGridLayout;
    this->layout->setSpacing(0);
    this->layout->setContentsMargins(0,0,0,0);
    this->setLayout(this->layout);

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
    this->stylesheet_button_flag          = QLatin1String(file_button_flag.readAll());
    this->stylesheet_button_mine          = QLatin1String(file_button_mine.readAll());
    this->stylesheet_button_mine_hit      = QLatin1String(file_button_mine_hit.readAll());
    this->stylesheet_button_revealed      = QLatin1String(file_button_revealed.readAll());
    this->stylesheet_button_unrevealed    = QLatin1String(file_button_unrevealed.readAll());
    this->stylesheet_button_1             = QLatin1String(file_button_1.readAll());
    this->stylesheet_button_2             = QLatin1String(file_button_2.readAll());
    this->stylesheet_button_3             = QLatin1String(file_button_3.readAll());
    this->stylesheet_button_4             = QLatin1String(file_button_4.readAll());
    this->stylesheet_button_5             = QLatin1String(file_button_5.readAll());
    this->stylesheet_button_6             = QLatin1String(file_button_6.readAll());
    this->stylesheet_button_7             = QLatin1String(file_button_7.readAll());
    this->stylesheet_button_8             = QLatin1String(file_button_8.readAll());
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
}

Field::~Field()
{
    for (int i=0; i <= cols; i++)
        delete[] this->cell[i];
    delete[] this->cell;
}

QVector<QVector<char>> Field::create2DVector()
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
    {
        this->cell[i] = new Cell[this->rows + 1];
    }
}

// place mines at random positions of this->minesVector:
void Field::fillMinesVector(const Common::Coords& userFirstInput)
{
    Common::Coords coords;
    int sizeOfField2DVector = this->cols * this->rows;
    QVector<int> tempVector;
    for (int i = 1; i <= sizeOfField2DVector; i++)
    {
        if (i != Common::CoordsToInt(userFirstInput, this->cols))
            tempVector.push_back(i);
    }

    std::random_shuffle(tempVector.begin(), tempVector.end());
    for (int i = 0; i < this->mines; i++)
    {
        coords = Common::intToCoords(tempVector.at(i), this->cols);
        this->mines2DVector[coords.col][coords.row] = 'X';
    }
}

void Field::addCells()
{
    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            this->cell[i][j].setStyleSheet(this->stylesheet_button_unrevealed);
            this->cell[i][j].setFixedSize(this->cellSize, this->cellSize);

            // create a vector holding structs of each button together with its coords:
            Common::Coords coordsTemp;
            coordsTemp.col = i;
            coordsTemp.row = j;
            buttonStruct structTemp;
            structTemp.coords = coordsTemp;
            structTemp.button = &this->cell[i][j];
            this->buttonStructVector.append(structTemp);

            this->layout->addWidget(&this->cell[i][j], j - 1, i - 1, 1, 1);
            connect(&this->cell[i][j], &Cell::doubleClicked, this, &Field::on_double_clicked);
            connect(&this->cell[i][j], &Cell::leftReleased, this, &Field::on_left_released);
            connect(&this->cell[i][j], &Cell::rightReleased, this, &Field::on_right_released);
        }
    }
}

// test coords if they contain a flag:
bool Field::isFlagSet(const Common::Coords& coords)
{
    if (this->field2DVector[coords.col][coords.row] == 'F')
        return true;
    else
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(const Common::Coords& coords)
{
    for (int i = 1; i < 8; i++)
    {
        if (this->field2DVector[coords.col][coords.row] == i + 48)
            return true;
    }
    return false;
}

// find neighbours of a cell at "coords" that hold a given content (passed by variable content)
QVector<Common::Coords> Field::findNeighbours(const QVector<QVector<char>>& temp2DVector, const Common::Coords& coords, const char& content)
{
    QVector<Common::Coords> findNeighboursReturn;
    QVector<QVector<int>> neighboursVector;

    neighboursVector.push_back({ -1, -1 });
    neighboursVector.push_back({  0, -1 });
    neighboursVector.push_back({  1, -1 });
    neighboursVector.push_back({  1,  0 });
    neighboursVector.push_back({  1,  1 });
    neighboursVector.push_back({  0,  1 });
    neighboursVector.push_back({ -1,  1 });
    neighboursVector.push_back({ -1,  0 });

    for (size_t x = 0; x < 8; ++x)
    {
        if (
            !(coords.col == 1 && neighboursVector[x][0] == -1) &&
            !(coords.row == 1 && neighboursVector[x][1] == -1) &&
            !(coords.col == this->cols && neighboursVector[x][0] == 1) &&
            !(coords.row == this->rows && neighboursVector[x][1] == 1)
            )
        {
            if (content == this->isNumber(coords))
            {
                for (int i = 1; i < 8; ++i)
                {
                    if (temp2DVector[coords.col + neighboursVector[x][0]][coords.row + neighboursVector[x][1]] == i)
                    {
                        Common::Coords coordsTemp;
                        coordsTemp.col = coords.col + neighboursVector[x][0];
                        coordsTemp.row = coords.row + neighboursVector[x][1];
                        findNeighboursReturn.push_back(coordsTemp);
                    }
                }
            }
            else if (temp2DVector[coords.col + neighboursVector[x][0]][coords.row + neighboursVector[x][1]] == content)
            {
                Common::Coords coordsTemp;
                coordsTemp.col = coords.col + neighboursVector[x][0];
                coordsTemp.row = coords.row + neighboursVector[x][1];
                findNeighboursReturn.push_back(coordsTemp);
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
        this->field2DVector[coords.col][coords.row] = '0';
        this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_revealed);
    }
    else
    {
        if (number == 1)
        {
            this->field2DVector[coords.col][coords.row] = '1';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_1);
        }
        if (number == 2)
        {
            this->field2DVector[coords.col][coords.row] = '2';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_2);
        }
        if (number == 3)
        {
            this->field2DVector[coords.col][coords.row] = '3';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_3);
        }
        if (number == 4)
        {
            this->field2DVector[coords.col][coords.row] = '4';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_4);
        }
        if (number == 5)
        {
            this->field2DVector[coords.col][coords.row] = '5';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_5);
        }
        if (number == 6)
        {
            this->field2DVector[coords.col][coords.row] = '6';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_6);
        }
        if (number == 7)
        {
            this->field2DVector[coords.col][coords.row] = '7';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_7);
        }
        if (number == 8)
        {
            this->field2DVector[coords.col][coords.row] = '8';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_8);
        }
    }
}

// used for both winning and losing a game (mode = "win" or "lose"):
void Field::gameOver(const Common::Coords& coords, const QString& mode)
{
    this->minesLeft = 0;
    this->isGameOver = true;
    emit this->minesleft_changed(this->minesLeft);
    emit this->game_over(mode);

    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            if (this->mines2DVector[i][j] == 'X')
                this->cell[i][j].setStyleSheet(this->stylesheet_button_mine);
            else
                this->cell[i][j].setStyleSheet(this->stylesheet_button_revealed);

            if (this->mines2DVector[i][j] != 'X' && this->mines2DVector[i][j] != 'H')
            {
                Common::Coords coordsTemp;
                coordsTemp.col = i;
                coordsTemp.row = j;
                QVector<Common::Coords> neighboursMinesVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'X');
                QVector<Common::Coords> neighboursMinesHitVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'H');
                this->printNumber(coordsTemp, static_cast<int>(neighboursMinesVector.size() + neighboursMinesHitVector.size()));
            }
            else if (this->mines2DVector[i][j] == 'X')
            {
                this->cell[i][j].setStyleSheet(this->stylesheet_button_mine);
            }
            else if (this->mines2DVector[i][j] == 'H')
            {
                this->cell[i][j].setStyleSheet(this->stylesheet_button_mine_hit);
            }
        }
    }
    if (mode == "lose")
    {
        this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet_button_mine_hit);
    }
}

// automatically reveal all connected cells, as long as they have no neighbour mines:
void Field::autoReveal(const Common::Coords& coords, QVector<int>& poolVector)
{
    // create vector holding unrevealed neighbours:
    QVector<Common::Coords> neighboursUnrevealedVector;
    neighboursUnrevealedVector = this->findNeighbours(this->field2DVector, coords, ' ');

    for (int i = 0; i < neighboursUnrevealedVector.size(); ++i)
    {
        QVector<Common::Coords> neighboursMinesVector;
        neighboursMinesVector = this->findNeighbours(this->mines2DVector, neighboursUnrevealedVector.at(i), 'X');
        if (std::find(poolVector.begin(), poolVector.end(), Common::CoordsToInt(neighboursUnrevealedVector.at(i), this->cols)) == poolVector.end())
        {
            if (this->mines2DVector[neighboursUnrevealedVector.at(i).col][neighboursUnrevealedVector.at(i).row] != 'X')
            {
                QVector<Common::Coords> neighboursZerosVector;
                neighboursZerosVector = this->findNeighbours(this->field2DVector, neighboursUnrevealedVector.at(i), '0');
                if (neighboursZerosVector.size() == 0)
                {
                    continue;
                }
                else
                {
                    poolVector.push_back(Common::CoordsToInt(neighboursUnrevealedVector.at(i), this->cols));
                    this->printNumber(neighboursUnrevealedVector.at(i), neighboursMinesVector.size());
                    --this->countUnrevealed;
                }
            }
        }
        if (neighboursMinesVector.size() == 0)
        {
            this->autoReveal(neighboursUnrevealedVector.at(i), poolVector);
        }
    }
}

void Field::flagAutoReveal(const Common::Coords& coords)
{
    // create a new vector of surrounding flags:
    QVector<Common::Coords> flagRevealNeighboursFlagsVector;
    flagRevealNeighboursFlagsVector = this->findNeighbours(this->field2DVector, coords, 'F');

    // if player has placed some flags around coords:
    if (flagRevealNeighboursFlagsVector.size() != 0)
    {
        // create a new vector of surrounding mines:
        QVector<Common::Coords> flagRevealNeighboursMinesVector;
        flagRevealNeighboursMinesVector = this->findNeighbours(this->mines2DVector, coords, 'X');

        // only proceed if the flag number matches the number of actual surrounding mines:
        if (flagRevealNeighboursMinesVector.size() == flagRevealNeighboursFlagsVector.size())
        {
            // create a new vector of surrounding unrevealed squares:
            QVector<Common::Coords> flagRevealNeighboursUnrevealedVector;
            flagRevealNeighboursUnrevealedVector = this->findNeighbours(this->field2DVector, coords, ' ');

            // create a new empty vector for missed mines:
            QVector<Common::Coords> flagRevealMissedMinesVector;

            // for each unrevealed neighbour of coords check if the player has missed a mine
            // and add this mines position to flagRevealMissedMinesVector:
            for (int i = 0; i < flagRevealNeighboursUnrevealedVector.size(); ++i)
            {
                if (this->mines2DVector[flagRevealNeighboursUnrevealedVector.at(i).col][flagRevealNeighboursUnrevealedVector.at(i).row] == 'X')
                {
                    flagRevealMissedMinesVector.push_back(flagRevealNeighboursUnrevealedVector.at(i));
                }
            }
            // if there are missed mines, reveal the mines2DVector - player has lost:
            if (flagRevealMissedMinesVector.size() != 0)
            {
                for (int i = 0; i < flagRevealMissedMinesVector.size(); ++i)
                {
                    this->mines2DVector[flagRevealMissedMinesVector.at(i).col][flagRevealMissedMinesVector.at(i).row] = 'H';
                }
                Common::Coords dummyCoords;
                this->gameOver(dummyCoords, "lose");
            }
            // else if all flags are placed correctly:
            else
            {
                if (flagRevealNeighboursMinesVector.size() == flagRevealNeighboursFlagsVector.size())
                {
                    // create a pool of already revealed cells, to avoid double checks within autoReveal():
                    QVector<int> poolVector;
                    // for each unrevealed neighbour of coords, print the number of surrounding mines:
                    for (int i = 0; i < flagRevealNeighboursUnrevealedVector.size(); ++i)
                    {
                        Common::Coords coordsTemp;
                        coordsTemp.col = flagRevealNeighboursUnrevealedVector.at(i).col;
                        coordsTemp.row = flagRevealNeighboursUnrevealedVector.at(i).row;
                        QVector<Common::Coords> flagRevealNeighboursUnrevealedMinesVector;
                        flagRevealNeighboursUnrevealedMinesVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'X');

                        if (flagRevealNeighboursUnrevealedMinesVector.size() == 0)
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), Common::CoordsToInt(coordsTemp, this->cols)) == poolVector.end())
                            {
                                this->printNumber(coordsTemp, 0);
                                poolVector.push_back(Common::CoordsToInt(coordsTemp, this->cols));
                                --this->countUnrevealed;
                                this->autoReveal(coordsTemp, poolVector);
                            }
                        }
                        else
                        {
                            if (std::find(poolVector.begin(), poolVector.end(), Common::CoordsToInt(coordsTemp, this->cols)) == poolVector.end())
                            {
                                this->printNumber(coordsTemp, static_cast<int>(flagRevealNeighboursUnrevealedMinesVector.size()));
                                poolVector.push_back(Common::CoordsToInt(coordsTemp, this->cols));
                                --this->countUnrevealed;
                            }
                        }
                    }
                    if (flagRevealNeighboursUnrevealedVector.size() != 0)
                    {
                        int col = coords.col;
                        int row = coords.row;
                        if (this->field2DVector[col][row] != '0')
                            emit this->smiley_surprised();
                    }
                }
            }
        }
    }
}

// return the coords of a button:
// (my own approach with std::find_if to get rid of the complicated gridPosition() method)
Common::Coords Field::getButtonCoords(Cell *button)
{
    buttonStruct *structTemp = std::find_if(
                this->buttonStructVector.begin(),
                this->buttonStructVector.end(),
                [button] (buttonStruct& s) { return s.button == button; }
            );
    return structTemp->coords;
}

// return the coords of a button in the QGridLayout:
/*
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
*/

// handle left clicking on a cell:
void Field::on_left_released()
{
    if (this->isGameOver != true)
    {
        Cell *button = qobject_cast<Cell*>(sender());
        Common::Coords coordsTemp = this->getButtonCoords(button);

        if (this->field2DVector[coordsTemp.col][coordsTemp.row] != 'F' && (! this->isNumber(coordsTemp)))
        {
            QVector<Common::Coords> neighboursMinesVector;

            // fill mines2DVector with mines only once after users first guess:
            if (this->firstTurn)
                this->fillMinesVector(coordsTemp);

            // if user hit a mine, reveal the game field - game is lost:
            if (this->mines2DVector[coordsTemp.col][coordsTemp.row] == 'X')
            {
                this->gameOver(coordsTemp, "lose");
            }

            else
            {
                // reveal the players choice and place the number of surrounding mines in it:
                neighboursMinesVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'X');
                this->printNumber(coordsTemp, neighboursMinesVector.size());
                --this->countUnrevealed;
            }

            // automatically reveal all neighbours of squares with no neighbour mines:
            QVector<int> poolVector;
            this->autoReveal(coordsTemp, poolVector);
            this->firstTurn = false;
        }
    }

    // check if player has won:
    if (this->flagsCount + this->countUnrevealed == this->mines)
    {
        Common::Coords dummyCoords;
        this->gameOver(dummyCoords, "win");
    }
}

// place and remove flags with right click:
void Field::on_right_released()
{
    if (this->isGameOver != true)
    {
        Cell *button = qobject_cast<Cell *>(sender());
        Common::Coords coordsTemp = this->getButtonCoords(button);

        if (this->field2DVector[coordsTemp.col][coordsTemp.row] == ' ' || this->field2DVector[coordsTemp.col][coordsTemp.row] == 'F')
        {
            if (! this->isFlagSet(coordsTemp))
            {
                button->setStyleSheet(this->stylesheet_button_flag);
                this->field2DVector[coordsTemp.col][coordsTemp.row] = 'F';
                this->flagsCount++;
                this->minesLeft--;
                this->countUnrevealed--;
            }
            else
            {
                button->setStyleSheet(this->stylesheet_button_unrevealed);
                this->field2DVector[coordsTemp.col][coordsTemp.row] = ' ';
                this->flagsCount--;
                this->minesLeft++;
                this->countUnrevealed++;
            }
        }
    }
    emit this->minesleft_changed(this->minesLeft);
}

// auto-reveal safe cells with no neighbour mines by double clicking on a number
// (flags around the numbered button must be placed right):
void Field::on_double_clicked()
{
    if (this->isGameOver != true)
    {
        Cell *button = qobject_cast<Cell *>(sender());
        Common::Coords coordsTemp = this->getButtonCoords(button);
        if (this->isNumber(coordsTemp))
        {
            this->flagAutoReveal(coordsTemp);
        }
    }
    // check if player has won:
    if (this->flagsCount + this->countUnrevealed == this->mines)
    {
        Common::Coords dummyCoords;
        this->gameOver(dummyCoords, "win");
    }
}
