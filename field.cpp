#include <QDebug>
#include <QSizePolicy>
#include <QVector>
#include <QPixmap>

#include "button.h"
#include "common.h"
#include "field.h"

Field::Field(QWidget *parent, const int& cols_, const int& rows_, const int& mines_, const int& buttonSize_) : QWidget(parent)
{
    this->cols = cols_;
    this->rows = rows_;
    this->mines = mines_;
    this->buttonSize = buttonSize_;
    this->minesLeft = mines_;
    this->countUnrevealed = cols_ * rows_;
    this->flagsCount = 0;
    this->firstTurn = true;
    this->isGameOver = false;
    this->isSolverRunning = false;
    this->isNewGameRequested = false;
    emit this->minesleft_changed_signal(this->minesLeft);

    this->layout = new QGridLayout;
    this->layout->setSpacing(0);
    this->layout->setContentsMargins(0,0,0,0);
    this->setLayout(this->layout);

    this->create2DVectors();
}

Field::~Field()
{
    for (int i = 0; i <= cols; i++)
    {
        for (int j = 0; j <= rows; j++)
        {
            delete this->buttons2DVector[i][j];
            this->buttons2DVector[i][j] = nullptr;
        }
    }
}

void Field::setButtonIcon(Button *button, const QString &iconName)
{
    QPixmap icon(":/icons/png/" + iconName + ".png");
    QIcon ButtonIcon(icon);
    button->setIcon(icon);
    QSize size(buttonSize, buttonSize);
    button->setIconSize(size);
}

void Field::create2DVectors()
{
    for(int i = 0; i <= this->cols; i++)
    {
        QVector<char> charRow;
        QVector<Button*> buttonRow;
        for(int j = 0; j <= this->rows; j++)
        {
            charRow.push_back(' ');
            Button *button;
            button = new Button;
            buttonRow.push_back(button);

            if (i != 0 && j !=0)
            {
                // create a vector holding structs of each button together with its coords:
                buttonStruct structTemp;
                Common::Coords coordsTemp;
                coordsTemp.col = i;
                coordsTemp.row = j;
                structTemp.coords = coordsTemp;
                structTemp.button = button;
                this->buttonStructVector.append(structTemp);

                button->setMouseTracking(true);

                connect(button, &Button::double_clicked_signal, this, &Field::on_double_clicked);
                connect(button, &Button::left_pressed_signal, this, &Field::on_left_pressed);
                connect(button, &Button::left_released_signal, this, &Field::on_left_released);
                connect(button, &Button::right_released_signal, this, &Field::on_right_released);
                connect(button, &Button::left_pressed_and_moved_signal, this, &Field::on_left_pressed_and_moved);

                this->setButtonIcon(button, "button_unrevealed");

                button->setFixedSize(this->buttonSize, this->buttonSize);

                this->layout->addWidget(button, j - 1, i - 1, 1, 1);
            }

        }
        this->field2DVector.push_back(charRow);
        this->mines2DVector.push_back(charRow);
        this->buttons2DVector.push_back(buttonRow);
    }
}

// place mines at random positions of this->minesVector:
void Field::fillMines2DVector(const Common::Coords& userFirstInput)
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

// find neighbours of a button at "coords" that hold a given content (passed by variable content)
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
        if (! (coords.col == 1 && neighboursVector[x][0] == -1)
                && ! (coords.row == 1 && neighboursVector[x][1] == -1)
                && ! (coords.col == this->cols && neighboursVector[x][0] == 1)
                && ! (coords.row == this->rows && neighboursVector[x][1] == 1))
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
// put the number of surrounding mines in this->field2DVector[coords.col][coords.row]:
void Field::setNumber(const Common::Coords& coords, const int& number)
{
    for (int i = 0; i < 8; i++)
    {
        if (i == number)
        {
            this->field2DVector[coords.col][coords.row] = i + 48;
            break;
        }
    }
}

// print the number of surrounding mines in this->button[coords.col][coords.row]:
void Field::printNumber(const Common::Coords& coords, const int& number)
{
    if (number == 0)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_revealed");
    }
    else if (number == 1)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_1");
    }
    else if (number == 2)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_2");
    }
    else if (number == 3)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_3");
    }
    else if (number == 4)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_4");
    }
    else if (number == 5)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_5");
    }
    else if (number == 6)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_6");
    }
    else if (number == 7)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_7");
    }
    else if (number == 8)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_8");
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
                this->setButtonIcon(this->buttons2DVector[i][j], "button_mine");
            }
            else if (this->mines2DVector[i][j] == 'H')
            {
                this->setButtonIcon(this->buttons2DVector[i][j], "button_mine_hit");
            }
            else if (hasLost == true)
            {
                Common::Coords coordsTemp;
                coordsTemp.col = i;
                coordsTemp.row = j;
                if ( ! this->isNumber(coordsTemp))
                {
                    QVector<Common::Coords> neighboursMinesVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'X');
                    QVector<Common::Coords> neighboursMinesHitVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'H');
                    this->setNumber(coordsTemp, neighboursMinesVector.size() + neighboursMinesHitVector.size());
                    this->printNumber(coordsTemp, neighboursMinesVector.size() + neighboursMinesHitVector.size());
                }
            }
        }
    }
    if (mines2DVector[coords.col][coords.row] == 'X')
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "button_mine_hit");
    }
}

// automatically reveal all connected buttons, as long as they have no neighbour mines:
void Field::autoReveal(const Common::Coords& coords, QVector<int>& poolVector, bool aiReveal)
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
                    if (aiReveal && this->isNewGameRequested == false)
                    {
                        Common::sleep(5);
                    }
                    poolVector.push_back(Common::CoordsToInt(neighboursUnrevealedVector.at(i), this->cols));
                    this->setNumber(neighboursUnrevealedVector.at(i), neighboursMinesVector.size());
                    this->printNumber(neighboursUnrevealedVector.at(i), neighboursMinesVector.size());
                    --this->countUnrevealed;
                }
            }
            if (neighboursMinesVector.size() == 0)
            {
                this->autoReveal(neighboursUnrevealedVector.at(i), poolVector, aiReveal);
            }
        }
    }
}

void Field::flagAutoReveal(const Common::Coords& coords, bool hasCheated, bool aiReveal)
{
    // create a new vector of surrounding flags:
    QVector<Common::Coords> neighboursFlagsVector;
    neighboursFlagsVector = this->findNeighbours(this->field2DVector, coords, 'F');

    // if player has placed some flags around coords:
    if (neighboursFlagsVector.size() != 0)
    {
        // create a new vector of surrounding mines:
        QVector<Common::Coords> neighboursMinesVector;
        neighboursMinesVector = this->findNeighbours(this->mines2DVector, coords, 'X');

        // only proceed if the flag number matches the number of actual surrounding mines:
        if (neighboursMinesVector.size() == neighboursFlagsVector.size())
        {
            // create a new vector of surrounding unrevealed squares:
            QVector<Common::Coords> neighboursUnrevealedVector;
            neighboursUnrevealedVector = this->findNeighbours(this->field2DVector, coords, ' ');

            // create a new empty vector for missed mines:
            QVector<Common::Coords> missedMinesVector;

            // for each unrevealed neighbour of coords check if the player has missed a mine
            // and add this mines position to flagRevealMissedMinesVector:
            for (int i = 0; i < neighboursUnrevealedVector.size(); ++i)
            {
                if (this->mines2DVector[neighboursUnrevealedVector.at(i).col][neighboursUnrevealedVector.at(i).row] == 'X')
                {
                    missedMinesVector.push_back(neighboursUnrevealedVector.at(i));
                }
            }
            // if there are missed mines, reveal the mines2DVector - player has lost:
            if (missedMinesVector.size() != 0)
            {
                for (int i = 0; i < missedMinesVector.size(); ++i)
                {
                    this->mines2DVector[missedMinesVector.at(i).col][missedMinesVector.at(i).row] = 'H';
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
                for (int i = 0; i < neighboursUnrevealedVector.size(); ++i)
                {
                    Common::Coords coordsTemp;
                    coordsTemp.col = neighboursUnrevealedVector.at(i).col;
                    coordsTemp.row = neighboursUnrevealedVector.at(i).row;
                    QVector<Common::Coords> neighboursMinesOfNeighboursUnrevealedVector;
                    neighboursMinesOfNeighboursUnrevealedVector = this->findNeighbours(this->mines2DVector, coordsTemp, 'X');

                    if (std::find(poolVector.begin(), poolVector.end(), Common::CoordsToInt(coordsTemp, this->cols)) == poolVector.end())
                    {
                        if (this->flagsCount + this->countUnrevealed == this->mines)
                        {
                            Common::Coords dummyCoords;
                            this->gameOver(dummyCoords, false);
                        }
                        else if (neighboursMinesOfNeighboursUnrevealedVector.size() == 0)
                        {
                            this->setNumber(coordsTemp, 0);
                            this->printNumber(coordsTemp, 0);
                            poolVector.push_back(Common::CoordsToInt(coordsTemp, this->cols));
                            --this->countUnrevealed;
                            this->autoReveal(coordsTemp, poolVector, aiReveal);
                        }
                        else
                        {
                            this->setNumber(coordsTemp, neighboursMinesOfNeighboursUnrevealedVector.size());
                            this->printNumber(coordsTemp, neighboursMinesOfNeighboursUnrevealedVector.size());
                            poolVector.push_back(Common::CoordsToInt(coordsTemp, this->cols));
                            --this->countUnrevealed;
                        }
                    }
                }
            }
            if (neighboursUnrevealedVector.size() != 0
                    && this->flagsCount + this->countUnrevealed != this->mines
                    && hasCheated == false)
            {
                emit this->smiley_surprised_signal();
            }
        }
    }
}

// return the coords of a button:
// (my own approach with std::find_if to get rid of the complicated gridPosition() method)
Common::Coords Field::getCoordsFromButton(Button *button)
{
    buttonStruct *structTemp = std::find_if(
                this->buttonStructVector.begin(),
                this->buttonStructVector.end(),
                [button] (buttonStruct& s) { return s.button == button; }
            );
    return structTemp->coords;
    qDebug() << QString::number(structTemp->coords.col) << ", " << QString::number(structTemp->coords.row);
}

// find a button by its coords:
Button* Field::getButtonFromCoords(const Common::Coords &coords)
{
    buttonStruct *structTemp = std::find_if(
                this->buttonStructVector.begin(),
                this->buttonStructVector.end(),
                [coords] (buttonStruct& s) { return (s.coords.col == coords.col && s.coords.row == coords.row); }
            );
    return structTemp->button;
}

// return the coords of a button in the QGridLayout:
// (found here: https://stackoverflow.com/a/24192016)
/*
Common::Coords Field::gridPosition(Button* button)
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

Common::Coords Field::getCoordsFromMousePosition()
{
    Common::Coords returnCoords = this->leftPressedButtonCoords;

    if (this->currentMousePosition.x() < 0)
    {
        returnCoords.col = this->leftPressedButtonCoords.col - ((this->currentMousePosition.x() * -1) / this->buttonSize + 1);
    }
    else if (this->currentMousePosition.x() >= this->buttonSize)
    {
        returnCoords.col = this->leftPressedButtonCoords.col + ((this->currentMousePosition.x()) / this->buttonSize);
    }
    if (this->currentMousePosition.y() < 0)
    {
        returnCoords.row = this->leftPressedButtonCoords.row - ((this->currentMousePosition.y() * -1) / this->buttonSize + 1);
    }
    else if (this->currentMousePosition.y() >= this->buttonSize)
    {
        returnCoords.row = this->leftPressedButtonCoords.row + ((this->currentMousePosition.y()) / this->buttonSize);
    }

    return returnCoords;
}

void Field::on_left_pressed()
{
    if (this->isGameOver != true && this->isSolverRunning != true)
    {
        this->currentMousePosition.setX(0);
        this->currentMousePosition.setY(0);
        Button *button = qobject_cast<Button*>(sender());
        Common::Coords leftPressedCoords = this->getCoordsFromButton(button);

        this->leftPressedButtonCoords = leftPressedCoords;

        Common::Coords currentMouseCoords = this->getCoordsFromMousePosition();
        if (this->field2DVector[leftPressedCoords.col][leftPressedCoords.row] == ' '
                && leftPressedCoords.col == currentMouseCoords.col
                && leftPressedCoords.row == currentMouseCoords.row)
        {
            this->setButtonIcon(this->getButtonFromCoords(leftPressedCoords), "button_pressed");
        }
    }
}

// handle left clicking on a button:
void Field::on_left_released()
{
    if (this->isGameOver != true && this->isSolverRunning != true)
    {
        Button *button = qobject_cast<Button*>(sender());
        Common::Coords coordsTemp = this->getCoordsFromButton(button);
        Common::Coords currentMouseCoords = this->getCoordsFromMousePosition();
        if (currentMouseCoords.col != this->leftPressedButtonCoords.col
                || currentMouseCoords.row != this->leftPressedButtonCoords.row)
        {
            coordsTemp = this->lastButtonCoords;
        }
        if (this->field2DVector[coordsTemp.col][coordsTemp.row] == ' ')
        {
            if (currentMouseCoords.col >= 1
                    && currentMouseCoords.col <= this->cols
                    && currentMouseCoords.row >= 1
                    && currentMouseCoords.row <= this->rows)
            {
                QVector<Common::Coords> neighboursMinesVector;

                // fill mines2DVector with mines only once after users first guess:
                if (this->firstTurn)
                {
                    this->fillMines2DVector(coordsTemp);
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
                    this->setNumber(coordsTemp, neighboursMinesVector.size());
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
                    // automatically reveal all neighbours of squares with no neighbour mines:
                    this->autoReveal(coordsTemp, poolVector, false);
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
        }
    }
}

// place and remove flags with right click:
void Field::on_right_released()
{
    if (this->isGameOver != true && this->isSolverRunning != true)
    {
        Button *button = qobject_cast<Button *>(sender());
        Common::Coords coordsTemp = this->getCoordsFromButton(button);

        if (this->field2DVector[coordsTemp.col][coordsTemp.row] == ' ' || this->field2DVector[coordsTemp.col][coordsTemp.row] == 'F')
        {
            if (! this->isFlagSet(coordsTemp))
            {
                this->setButtonIcon(button, "button_flag");
                this->field2DVector[coordsTemp.col][coordsTemp.row] = 'F';
                this->flagsCount++;
                this->minesLeft--;
                this->countUnrevealed--;
            }
            else
            {
                this->setButtonIcon(button, "button_unrevealed");
                this->field2DVector[coordsTemp.col][coordsTemp.row] = ' ';
                this->flagsCount--;
                this->minesLeft++;
                this->countUnrevealed++;
            }
        }
        emit this->minesleft_changed_signal(this->minesLeft);
    }
}

// auto-reveal safe buttons with no neighbour mines by double clicking on a number
// (flags around the numbered button must be placed right):
void Field::on_double_clicked()
{
    if (this->isGameOver != true && this->isSolverRunning != true)
    {
        Button *button = qobject_cast<Button *>(sender());
        Common::Coords coordsTemp = this->getCoordsFromButton(button);
        if (this->isNumber(coordsTemp))
        {
            this->flagAutoReveal(coordsTemp, false, false);
        }

        // check if player has won:
        if (this->flagsCount + this->countUnrevealed == this->mines)
        {
            Common::Coords dummyCoords;
            this->gameOver(dummyCoords, false);
        }
    }
}

void Field::on_left_pressed_and_moved(QMouseEvent *e)
{
    if (this->isGameOver != true && e->buttons() == Qt::LeftButton && this->isSolverRunning != true)
    {
        this->currentMousePosition = e->pos();
        Common::Coords newButtonCoords = this->getCoordsFromMousePosition();

        if (this->lastButtonCoords.col == 0)
        {
            this->lastButtonCoords.col = this->leftPressedButtonCoords.col;
        }
        if (this->lastButtonCoords.row == 0)
        {
            this->lastButtonCoords.row = this->leftPressedButtonCoords.row;
        }

//        qDebug() << "last:" << QString::number(this->lastButtonCoords.col) << "," << QString::number(this->lastButtonCoords.row) <<
//                    "new:"  << QString::number(newButtonCoords.col) << "," << QString::number(newButtonCoords.row);

        if (newButtonCoords.col < 1
                || newButtonCoords.col > this->cols
                || newButtonCoords.row < 1
                || newButtonCoords.row > this->rows)
        {
            if (this->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
            {
                this->setButtonIcon(this->getButtonFromCoords(this->lastButtonCoords), "button_unrevealed");
            }

            if (newButtonCoords.col < 1)
            {
                this->lastButtonCoords.col = 0;
            }
            else if (newButtonCoords.col > this->cols)
            {
                this->lastButtonCoords.col = this->cols - 1;
            }
            if (newButtonCoords.row < 1)
            {
                this->lastButtonCoords.row = 0;
            }
            else if (newButtonCoords.row > this->rows)
            {
                this->lastButtonCoords.row = this->rows - 1;
            }
        }
        else if (this->lastButtonCoords.col >= 1
                 && this->lastButtonCoords.row >= 1
                 && this->lastButtonCoords.col <= this->cols
                 && this->lastButtonCoords.row <= this->rows)
        {
            if (this->lastButtonCoords.col != newButtonCoords.col || this->lastButtonCoords.row != newButtonCoords.row)
            {
                if (this->field2DVector[this->leftPressedButtonCoords.col][this->leftPressedButtonCoords.row] == ' ')
                {
                    this->setButtonIcon(this->getButtonFromCoords(this->leftPressedButtonCoords), "button_unrevealed");
                }
                if (this->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
                {
                    this->setButtonIcon(this->getButtonFromCoords(this->lastButtonCoords), "button_unrevealed");
                }
                if (this->field2DVector[newButtonCoords.col][newButtonCoords.row] == ' ')
                {
                    this->setButtonIcon(this->getButtonFromCoords(newButtonCoords), "button_pressed");
                }
                this->lastButtonCoords.col = newButtonCoords.col;
                this->lastButtonCoords.row = newButtonCoords.row;
            }
        }
    }
}

void Field::place_flag_slot(const Common::Coords& coords)
{
    this->field2DVector[coords.col][coords.row] = 'F';
    this->setButtonIcon(this->getButtonFromCoords(coords), "button_flag");
    this->flagsCount++;
    this->minesLeft--;
    this->countUnrevealed--;
    emit this->minesleft_changed_signal(this->minesLeft);
}
