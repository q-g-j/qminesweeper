#include <QApplication>
#include <QDebug>
#include <QSizePolicy>
#include <QVector>

#include "cell.h"
#include "common.h"
#include "field.h"

Field::Field(QWidget *parent, Stylesheet *stylesheet_, const int& cols_, const int& rows_, const int& mines_, const int& cellSize_) : QWidget(parent)
{
    this->stylesheet = stylesheet_;
    this->cols = cols_;
    this->rows = rows_;
    this->mines = mines_;
    this->cellSize = cellSize_;
    this->minesLeft = mines_;
    this->countUnrevealed = cols_ * rows_;
    this->flagsCount = 0;
    this->firstTurn = true;
    this->isGameOver = false;
    this->hasPressedAndMoved = false;
    this->field2DVector = this->create2DVector();
    this->mines2DVector = this->create2DVector();
    this->createCells();
    emit this->minesleft_changed_signal(this->minesLeft);

    this->layout = new QGridLayout;
    this->layout->setSpacing(0);
    this->layout->setContentsMargins(0,0,0,0);
    this->setLayout(this->layout);
}

Field::~Field()
{
    for (int i=0; i <= cols; i++)
    {
        delete[] this->cell[i];
        this->cell[i] = nullptr;
    }
    delete[] this->cell;    
    this->cell = nullptr;
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
    int sizeOfField = this->cols * this->rows;
    QVector<int> tempVector;
    for (int i = 1; i <= sizeOfField; i++)
    {
        if (i != Common::CoordsToInt(userFirstInput, this->cols))
        {
            tempVector.push_back(i);
        }
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
            this->cell[i][j].setStyleSheet(this->stylesheet->stylesheet_button_unrevealed);
            this->cell[i][j].setFixedSize(this->cellSize, this->cellSize);
            this->layout->addWidget(&this->cell[i][j], j - 1, i - 1, 1, 1);

            // create a vector holding structs of each button together with its coords:
            Common::Coords coordsTemp;
            coordsTemp.col = i;
            coordsTemp.row = j;
            buttonStruct structTemp;
            structTemp.coords = coordsTemp;
            structTemp.button = &this->cell[i][j];
            this->buttonsVector.append(structTemp);

            this->cell[i][j].setMouseTracking(true);

            connect(&this->cell[i][j], &Cell::double_clicked_signal, this, &Field::on_double_clicked);
            connect(&this->cell[i][j], &Cell::left_pressed_signal, this, &Field::on_left_pressed);
            connect(&this->cell[i][j], &Cell::left_released_signal, this, &Field::on_left_released);
            connect(&this->cell[i][j], &Cell::right_released_signal, this, &Field::on_right_released);
            connect(&this->cell[i][j], &Cell::left_pressed_and_moved_signal, this, &Field::on_left_pressed_and_moved);
        }
    }
}

// test coords if they contain a flag:
bool Field::isFlagSet(const Common::Coords& coords)
{
    if (this->field2DVector[coords.col][coords.row] == 'F')
    {
        return true;
    }
    else
    {
        return false;
    }
}

// test coords if they contain a number:
bool Field::isNumber(const Common::Coords& coords)
{
    for (int i = 0; i < 8; i++)
    {
        if (this->field2DVector[coords.col][coords.row] == i + 48)
        {
            return true;
        }
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
        this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_revealed);
    }
    else
    {
        if (number == 1)
        {
            this->field2DVector[coords.col][coords.row] = '1';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_1);
        }
        else if (number == 2)
        {
            this->field2DVector[coords.col][coords.row] = '2';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_2);
        }
        else if (number == 3)
        {
            this->field2DVector[coords.col][coords.row] = '3';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_3);
        }
        else if (number == 4)
        {
            this->field2DVector[coords.col][coords.row] = '4';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_4);
        }
        else if (number == 5)
        {
            this->field2DVector[coords.col][coords.row] = '5';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_5);
        }
        else if (number == 6)
        {
            this->field2DVector[coords.col][coords.row] = '6';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_6);
        }
        else if (number == 7)
        {
            this->field2DVector[coords.col][coords.row] = '7';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_7);
        }
        else if (number == 8)
        {
            this->field2DVector[coords.col][coords.row] = '8';
            this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_8);
        }
    }
}

// used for both winning and losing a game:
void Field::gameOver(const Common::Coords& coords, bool hasLost)
{
    this->isGameOver = true;
    this->minesLeft = 0;
    emit this->minesleft_changed_signal(this->minesLeft);
    emit this->game_over_signal(hasLost);

    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            if (this->mines2DVector[i][j] == 'X')
            {
                this->cell[i][j].setStyleSheet(this->stylesheet->stylesheet_button_mine);
            }
            else if (this->mines2DVector[i][j] == '0')
            {
                this->cell[i][j].setStyleSheet(this->stylesheet->stylesheet_button_revealed);
            }
            else if (this->mines2DVector[i][j] == 'H')
            {
                this->cell[i][j].setStyleSheet(this->stylesheet->stylesheet_button_mine_hit);
            }
            else if (hasLost == true)
            {
                Common::Coords coordsTemp;
                coordsTemp.col = i;
                coordsTemp.row = j;
                QVector<Common::Coords> neighboursMinesVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'X');
                QVector<Common::Coords> neighboursMinesHitVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'H');
                this->printNumber(coordsTemp, static_cast<int>(neighboursMinesVector.size() + neighboursMinesHitVector.size()));
            }
        }
    }
    if (mines2DVector[coords.col][coords.row] == 'X')
    {
        this->cell[coords.col][coords.row].setStyleSheet(this->stylesheet->stylesheet_button_mine_hit);
    }
}

// automatically reveal all connected cells, as long as they have no neighbour mines:
void Field::autoReveal(const Common::Coords& coords, QVector<int>& poolVector)
{
    // create vector holding unrevealed neighbours:
    QVector<Common::Coords> neighboursUnrevealedVector;
    neighboursUnrevealedVector = this->findNeighbours(this->field2DVector, coords, ' ');

    // check if player has won:
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
                this->gameOver(dummyCoords, true);
            }
            // else if all flags are placed correctly:
            else
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

                    if (std::find(poolVector.begin(), poolVector.end(), Common::CoordsToInt(coordsTemp, this->cols)) == poolVector.end())
                    {
                        if (this->flagsCount + this->countUnrevealed == this->mines)
                        {
                            Common::Coords dummyCoords;
                            this->gameOver(dummyCoords, false);
                        }
                        else if (flagRevealNeighboursUnrevealedMinesVector.size() == 0)
                        {
                            this->printNumber(coordsTemp, 0);
                            poolVector.push_back(Common::CoordsToInt(coordsTemp, this->cols));
                            --this->countUnrevealed;
                            this->autoReveal(coordsTemp, poolVector);
                        }
                        else
                        {
                            this->printNumber(coordsTemp, static_cast<int>(flagRevealNeighboursUnrevealedMinesVector.size()));
                            poolVector.push_back(Common::CoordsToInt(coordsTemp, this->cols));
                            --this->countUnrevealed;
                        }
                    }
                }
            }
            if (flagRevealNeighboursUnrevealedVector.size() != 0)
            {
                if (this->flagsCount + this->countUnrevealed != this->mines)
                {
                    emit this->smiley_surprised_signal();
                }
            }
        }
    }
}

// return the coords of a button:
// (my own approach with std::find_if to get rid of the complicated gridPosition() method)
Common::Coords Field::getCoordsFromButton(Cell *button)
{
    buttonStruct *structTemp = std::find_if(
                this->buttonsVector.begin(),
                this->buttonsVector.end(),
                [button] (buttonStruct& s) { return s.button == button; }
            );
    return structTemp->coords;
    qDebug() << QString::number(structTemp->coords.col) << ", " << QString::number(structTemp->coords.row);
}

// return the coords of a button:
// (my own approach with std::find_if to get rid of the complicated gridPosition() method)
Cell* Field::getButtonFromCoords(const Common::Coords &coords)
{
    buttonStruct *structTemp = std::find_if(
                this->buttonsVector.begin(),
                this->buttonsVector.end(),
                [coords] (buttonStruct& s) { return (s.coords.col == coords.col && s.coords.row == coords.row); }
            );
    return structTemp->button;
}

// return the coords of a button in the QGridLayout:
// (found here: https://stackoverflow.com/a/24192016)
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

void Field::on_left_pressed()
{
    if (this->isGameOver != true)
    {
        this->currentMousePosition.setX(0);
        this->currentMousePosition.setY(0);
        Cell *button = qobject_cast<Cell*>(sender());
        Common::Coords coordsTemp = this->getCoordsFromButton(button);

        this->pressedButtonCoords = coordsTemp;

        if (this->field2DVector[coordsTemp.col][coordsTemp.row] != 'F' && ! (this->isNumber(coordsTemp)))
        {
            this->getButtonFromCoords(coordsTemp)->setStyleSheet(this->stylesheet->stylesheet_button_pressed);
        }
    }
}

// handle left clicking on a cell:
void Field::on_left_released()
{
    if (this->isGameOver != true)
    {
        Cell *button = qobject_cast<Cell*>(sender());
        Common::Coords coordsTemp = this->getCoordsFromButton(button);
        if (this->hasPressedAndMoved)
        {
            coordsTemp = this->lastButtonCoords;
        }
        if (this->field2DVector[coordsTemp.col][coordsTemp.row] == ' ')
        {
            Common::Coords currentMouseCoords = this->getCoordsFromRelativePosition(this->currentMousePosition, this->pressedButtonCoords);
            if (currentMouseCoords.col >= 1 &&
                    currentMouseCoords.col <= this->cols &&
                    currentMouseCoords.row >= 1 &&
                    currentMouseCoords.row <= this->rows)
            {
                QVector<Common::Coords> neighboursMinesVector;

                // fill mines2DVector with mines only once after users first guess:
                if (this->firstTurn)
                {
                    this->fillMinesVector(coordsTemp);
                    emit this->game_started_signal();
                }

                // if user hit a mine, reveal the game field - game is lost:
                if (this->mines2DVector[coordsTemp.col][coordsTemp.row] == 'X')
                {
                    this->gameOver(coordsTemp, true);
                }

                else
                {
                    // reveal the players choice and place the number of surrounding mines in it:
                    neighboursMinesVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'X');
                    this->printNumber(coordsTemp, neighboursMinesVector.size());
                    --this->countUnrevealed;

                    // check if player has won:
                    if (this->flagsCount + this->countUnrevealed == this->mines)
                    {
                        Common::Coords dummyCoords;
                        this->gameOver(dummyCoords, false);
                    }
                }
                if (this->isGameOver != true)
                {
                    // automatically reveal all neighbours of squares with no neighbour mines:
                    QVector<int> poolVector;
                    this->autoReveal(coordsTemp, poolVector);
                    this->firstTurn = false;

                    // check if player has won:
                    if (this->flagsCount + this->countUnrevealed == this->mines)
                    {
                        Common::Coords dummyCoords;
                        this->gameOver(dummyCoords, false);
                    }
                }
                emit this->smiley_surprised_signal();
            }
            else if (this->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
            {
                this->getButtonFromCoords(this->lastButtonCoords)->setStyleSheet(this->stylesheet->stylesheet_button_unrevealed);
            }
        }
        this->hasPressedAndMoved = false;
    }
}

// place and remove flags with right click:
void Field::on_right_released()
{
    if (this->isGameOver != true)
    {
        Cell *button = qobject_cast<Cell *>(sender());
        Common::Coords coordsTemp = this->getCoordsFromButton(button);

        if (this->field2DVector[coordsTemp.col][coordsTemp.row] == ' ' || this->field2DVector[coordsTemp.col][coordsTemp.row] == 'F')
        {
            if (! this->isFlagSet(coordsTemp))
            {
                button->setStyleSheet(this->stylesheet->stylesheet_button_flag);
                this->field2DVector[coordsTemp.col][coordsTemp.row] = 'F';
                this->flagsCount++;
                this->minesLeft--;
                this->countUnrevealed--;
            }
            else
            {
                button->setStyleSheet(this->stylesheet->stylesheet_button_unrevealed);
                this->field2DVector[coordsTemp.col][coordsTemp.row] = ' ';
                this->flagsCount--;
                this->minesLeft++;
                this->countUnrevealed++;
            }
        }
        emit this->minesleft_changed_signal(this->minesLeft);
    }
}

// auto-reveal safe cells with no neighbour mines by double clicking on a number
// (flags around the numbered button must be placed right):
void Field::on_double_clicked()
{
    if (this->isGameOver != true)
    {
        Cell *button = qobject_cast<Cell *>(sender());
        Common::Coords coordsTemp = this->getCoordsFromButton(button);
        if (this->isNumber(coordsTemp))
        {
            this->flagAutoReveal(coordsTemp);
        }

        // check if player has won:
        if (this->flagsCount + this->countUnrevealed == this->mines)
        {
            Common::Coords dummyCoords;
            this->gameOver(dummyCoords, false);
        }
    }
}

Common::Coords Field::getCoordsFromRelativePosition(QPoint& position, Common::Coords& buttonCoords)
{
    Common::Coords returnCoords = buttonCoords;

    if (position.x() < 0)
    {
        returnCoords.col = buttonCoords.col - ((position.x() * -1) / 25 + 1);
    }
    else if (position.x() >= this->cellSize)
    {
        returnCoords.col = buttonCoords.col + ((position.x()) / 25);
    }
    if (position.y() < 0)
    {
        returnCoords.row = buttonCoords.row - ((position.y() * -1) / 25 + 1);
    }
    else if (position.y() >= this->cellSize)
    {
        returnCoords.row = buttonCoords.row + ((position.y()) / 25);
    }

    return returnCoords;
}

void Field::on_left_pressed_and_moved(QMouseEvent *e)
{
    if (this->isGameOver != true && e->buttons() == Qt::LeftButton)
    {
//        if (this->field2DVector[this->pressedButtonCoords.col][this->pressedButtonCoords.row] == ' ')
        {
            this->currentMousePosition = e->pos();
            Common::Coords newButtonCoords = this->getCoordsFromRelativePosition(this->currentMousePosition, this->pressedButtonCoords);

            if (this->lastButtonCoords.col == 0)
            {
                this->lastButtonCoords.col = this->pressedButtonCoords.col;
            }
            if (this->lastButtonCoords.row == 0)
            {
                this->lastButtonCoords.row = this->pressedButtonCoords.row;
            }

            if (newButtonCoords.col >= 1 && newButtonCoords.col <= this->cols && newButtonCoords.row >= 1 && newButtonCoords.row <= this->rows)
            {
                if (this->lastButtonCoords.col != newButtonCoords.col || this->lastButtonCoords.row != newButtonCoords.row)
                {
                    if (this->field2DVector[this->pressedButtonCoords.col][this->pressedButtonCoords.row] == ' ')
                    {
                        this->getButtonFromCoords(this->pressedButtonCoords)->setStyleSheet(this->stylesheet->stylesheet_button_unrevealed);
                    }
                    if (this->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
                    {
                        this->getButtonFromCoords(this->lastButtonCoords)->setStyleSheet(this->stylesheet->stylesheet_button_unrevealed);
                    }
                    if (this->field2DVector[newButtonCoords.col][newButtonCoords.row] == ' ')
                    {
                        this->getButtonFromCoords(newButtonCoords)->setStyleSheet(this->stylesheet->stylesheet_button_pressed);
                    }
                }
                this->lastButtonCoords.col = newButtonCoords.col;
                this->lastButtonCoords.row = newButtonCoords.row;
                this->hasPressedAndMoved = true;
            }
            else
            {
                if (this->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
                {
                    this->getButtonFromCoords(this->lastButtonCoords)->setStyleSheet(this->stylesheet->stylesheet_button_unrevealed);
                }
            }
        }
    }
}
