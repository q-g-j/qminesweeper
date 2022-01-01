#include <QDebug>
#include <QSizePolicy>
#include <QVector>
#include <QPixmap>

#include "button.h"
#include "common.h"
#include "field.h"

Field::Field(QWidget *parent, const qint32& cols_, const qint32& rows_, const qint32& mines_, const qint32& buttonSize_) : QWidget(parent)
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
}

Field::~Field()
{
    for (qint32 i = 0; i <= cols; i++)
    {
        for (qint32 j = 0; j <= rows; j++)
        {
            delete this->buttons2DVector[i][j];
            this->buttons2DVector[i][j] = nullptr;
        }
    }
    delete this->layout;
    this->layout = nullptr;
}

void Field::setButtonIcon(Button *button, const QString &iconName)
{
    QPixmap icon(":/icons/png/button_" + iconName + ".png");
    button->setIcon(icon);
    QSize size(buttonSize, buttonSize);
    button->setIconSize(size);
}

void Field::create2DVectors()
{
    for (qint32 i = 0; i <= this->cols; i++)
    {
        QVector<char> charRow;
        QVector<Button*> buttonRow;
        for(qint32 j = 0; j <= this->rows; j++)
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
                button->setFixedSize(this->buttonSize, this->buttonSize);
                button->setAttribute(Qt::WA_LayoutUsesWidgetRect);
                this->setButtonIcon(button, "unrevealed");
                this->layout->addWidget(button, j - 1, i - 1, 1, 1);
                emit this->connect_button_signal(button);
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
    qint32 sizeOfField = this->cols * this->rows;
    QVector<qint32> tempVector = Common::randomShuffle(sizeOfField, Common::CoordsToInt(userFirstInput, this->cols));

    for (qint32 i = 0; i < this->mines; i++)
    {
        coords = Common::intToCoords(tempVector.at(i), this->cols);
        this->mines2DVector[coords.col][coords.row] = 'X';
    }
}

// test coords if they contain a number:
bool Field::isNumber(const Common::Coords& coords)
{
    for (qint32 i = 0; i < 8; i++)
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
    QVector<QVector<qint16>> neighboursVector;

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
                for (qint32 i = 1; i < 8; ++i)
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
void Field::setNumber(const Common::Coords& coords, const qint32& number)
{
    for (qint32 i = 0; i < 8; i++)
    {
        if (i == number)
        {
            this->field2DVector[coords.col][coords.row] = i + 48;
            break;
        }
    }
}

// print the number of surrounding mines in this->button[coords.col][coords.row]:
void Field::printNumber(const Common::Coords& coords, const qint32& number)
{
    if (number == 0)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "revealed");
    }
    else if (number == 1)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "1");
    }
    else if (number == 2)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "2");
    }
    else if (number == 3)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "3");
    }
    else if (number == 4)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "4");
    }
    else if (number == 5)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "5");
    }
    else if (number == 6)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "6");
    }
    else if (number == 7)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "7");
    }
    else if (number == 8)
    {
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "8");
    }
}

// used for both winning and losing a game:
void Field::gameOver(const Common::Coords& coords, bool hasLost)
{
    this->isGameOver = true;
    this->minesLeft = 0;
    emit this->minesleft_changed_signal(this->minesLeft);
    emit this->game_over_signal(hasLost);

    for (qint32 i = 1; i <= this->cols; i++)
    {
        for (qint32 j = 1; j <= this->rows; j++)
        {
            if (this->mines2DVector[i][j] == 'X')
            {
                this->setButtonIcon(this->buttons2DVector[i][j], "mine");
            }
            else if (this->mines2DVector[i][j] == 'H')
            {
                this->setButtonIcon(this->buttons2DVector[i][j], "mine_hit");
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
        this->setButtonIcon(this->buttons2DVector[coords.col][coords.row], "mine_hit");
    }
}

// automatically reveal all connected buttons, as long as they have no neighbour mines:
void Field::autoReveal(const Common::Coords& coords, QVector<qint32>& poolVector, bool aiReveal)
{
    // create vector holding unrevealed neighbours:
    QVector<Common::Coords> neighboursUnrevealedVector;
    neighboursUnrevealedVector = this->findNeighbours(this->field2DVector, coords, ' ');

    for (qint32 i = 0; i < neighboursUnrevealedVector.size(); ++i)
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
            for (qint32 i = 0; i < neighboursUnrevealedVector.size(); ++i)
            {
                if (this->mines2DVector[neighboursUnrevealedVector.at(i).col][neighboursUnrevealedVector.at(i).row] == 'X')
                {
                    missedMinesVector.push_back(neighboursUnrevealedVector.at(i));
                }
            }
            // if there are missed mines, reveal the mines2DVector - player has lost:
            if (missedMinesVector.size() != 0)
            {
                for (qint32 i = 0; i < missedMinesVector.size(); ++i)
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
                QVector<qint32> poolVector;
                // for each unrevealed neighbour of coords, print the number of surrounding mines:
                for (qint32 i = 0; i < neighboursUnrevealedVector.size(); ++i)
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

void Field::solver_place_flag_slot(const Common::Coords& coords)
{
    this->field2DVector[coords.col][coords.row] = 'F';
    this->setButtonIcon(this->getButtonFromCoords(coords), "flag");
    this->flagsCount++;
    this->minesLeft--;
    this->countUnrevealed--;
    emit this->minesleft_changed_signal(this->minesLeft);
}
