#include <QDebug>
#include <QSizePolicy>
#include <QVector>

#include "button.h"
#include "common.h"
#include "field.h"

Field::Field(
        QWidget *parent,
        const quint16& cols_,
        const quint16& rows_,
        const quint16& mines_,
        const quint16& buttonSize_
        ) : QWidget(parent)
{
    this->cols = cols_;
    this->rows = rows_;
    this->mines = mines_;
    this->buttonSize = buttonSize_;
    this->minesLeft = mines_;
    this->countUnrevealed = cols_ * rows_;

    emit this->minesleft_changed_signal(this->minesLeft);

    this->layout = new QGridLayout;
    this->layout->setSpacing(0);
    this->layout->setContentsMargins(0,0,0,0);
    this->setLayout(this->layout);

    this->button_1.load(":/icons/png/button_1");
    this->button_2.load(":/icons/png/button_2");
    this->button_3.load(":/icons/png/button_3");
    this->button_4.load(":/icons/png/button_4");
    this->button_5.load(":/icons/png/button_5");
    this->button_6.load(":/icons/png/button_6");
    this->button_7.load(":/icons/png/button_7");
    this->button_8.load(":/icons/png/button_8");
    this->button_revealed.load(":/icons/png/button_revealed");
    this->button_unrevealed.load(":/icons/png/button_unrevealed");
    this->button_pressed.load(":/icons/png/button_pressed");
    this->button_mine.load(":/icons/png/button_mine");
    this->button_mine_hit.load(":/icons/png/button_mine_hit");
    this->button_flag.load(":/icons/png/button_flag");
}

Field::~Field()
{
    for (quint16 i = 0; i <= cols; i++)
    {
        for (quint16 j = 0; j <= rows; j++)
        {
            delete this->buttons2DVector[i][j];
            this->buttons2DVector[i][j] = nullptr;
        }
    }
    delete this->layout;
    this->layout = nullptr;
}

void Field::setButtonIcon(
        Button* button,
        const QPixmap& iconName
        )
{
    button->setPixmap(iconName);
    QSize size(this->buttonSize, this->buttonSize);
    button->setScaledContents( true );
}

void Field::create2DVectors()
{
    for (quint16 i = 0; i <= this->cols; i++)
    {
        QVector<char> charRow;
        QVector<Button*> buttonRow;
        for (quint16 j = 0; j <= this->rows; j++)
        {
            charRow.push_back(' ');
            Button *button;
            button = new Button;
            buttonRow.push_back(button);

            if (i != 0 && j != 0)
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
                this->setButtonIcon(
                            button,
                            this->button_unrevealed
                            );
                button->setScaledContents(true);
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
    QVector<quint16> tempVector = Common::randomShuffle(
                this->cols * this->rows,
                Common::CoordsToInt(userFirstInput, this->cols)
                );

    for (quint16 i = 0; i < this->mines; i++)
    {
        Common::Coords coords;
        coords = Common::intToCoords(
                    tempVector.at(i),
                    this->cols
                    );
        this->mines2DVector[coords.col][coords.row] = 'X';
    }
}

// test coords if they contain a number:
bool Field::isNumber(const Common::Coords& coords)
{
    for (quint16 i = 0; i < 8; i++)
    {
        if (this->field2DVector[coords.col][coords.row] == i + 48)
        {
            return true;
        }
    }
    return false;
}

// find neighbours of a button at "coords" that hold a given content (passed by variable content)
QVector<Common::Coords> Field::findNeighbours(
        const QVector<QVector<char>>& temp2DVector,
        const Common::Coords& coords,
        const char& content
        )
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
        if (
                ! (coords.col == 1 && neighboursVector[x][0] == -1)
                && ! (coords.row == 1 && neighboursVector[x][1] == -1)
                && ! (coords.col == this->cols && neighboursVector[x][0] == 1)
                && ! (coords.row == this->rows && neighboursVector[x][1] == 1)
                )
        {
            if (content == this->isNumber(coords))
            {
                for (quint16 i = 1; i < 8; ++i)
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
void Field::setNumber(
        const Common::Coords& coords,
        const quint16& number
        )
{
    for (quint16 i = 0; i < 8; i++)
    {
        if (i == number)
        {
            this->field2DVector[coords.col][coords.row] = i + 48;
            break;
        }
    }
}

// print the number of surrounding mines in this->button[coords.col][coords.row]:
void Field::printNumber(
        const Common::Coords& coords,
        const quint16& number
        )
{
    Button *button = this->getButtonFromCoords(coords);
    if (number == 0)
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                    this->button_revealed
                    );
        button->setFocusPolicy(Qt::NoFocus);
    }
    else if (number == 1)
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                    this->button_1
                    );
        button->setFocusPolicy(Qt::NoFocus);
    }
    else if (number == 2)
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                    this->button_2
                    );
        button->setFocusPolicy(Qt::NoFocus);
    }
    else if (number == 3)
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                            this->button_3
                            );
        button->setFocusPolicy(Qt::NoFocus);
    }
    else if (number == 4)
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                    this->button_4
                    );
        button->setFocusPolicy(Qt::NoFocus);
    }
    else if (number == 5)
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                    this->button_5
                    );
        button->setFocusPolicy(Qt::NoFocus);
    }
    else if (number == 6)
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                    this->button_6
                    );
        button->setFocusPolicy(Qt::NoFocus);
    }
    else if (number == 7)
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                    this->button_7
                    );
        button->setFocusPolicy(Qt::NoFocus);
    }
    else if (number == 8)
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                    this->button_8
                    );
        button->setFocusPolicy(Qt::NoFocus);
    }
}

// used for both winning and losing a game:
void Field::gameOver(
        const Common::Coords& coords,
        bool hasLost
        )
{
    this->isGameOver = true;
    this->minesLeft = 0;
    emit this->minesleft_changed_signal(this->minesLeft);
    emit this->game_over_signal(hasLost);

    for (quint16 i = 1; i <= this->cols; i++)
    {
        for (quint16 j = 1; j <= this->rows; j++)
        {
            if (this->mines2DVector[i][j] == 'X')
            {
                this->setButtonIcon(
                            this->buttons2DVector[i][j],
                            this->button_mine
                            );
            }
            else if (this->mines2DVector[i][j] == 'H')
            {
                this->setButtonIcon(
                            this->buttons2DVector[i][j],
                            this->button_mine_hit
                            );
            }
            else if (hasLost == true)
            {
                Common::Coords coordsTemp;
                coordsTemp.col = i;
                coordsTemp.row = j;
                if (this->isNumber(coordsTemp) == false)
                {
                    QVector<Common::Coords> neighboursMinesVector = this->findNeighbours(
                                this->mines2DVector,
                                coordsTemp,
                                'X'
                                );
                    QVector<Common::Coords> neighboursMinesHitVector = this->findNeighbours(
                                this->mines2DVector,
                                coordsTemp,
                                'H'
                                );
                    this->setNumber(
                                coordsTemp,
                                neighboursMinesVector.size() + neighboursMinesHitVector.size()
                                );
                    this->printNumber(
                                coordsTemp,
                                neighboursMinesVector.size() + neighboursMinesHitVector.size()
                                );
                }
            }
        }
    }
    if (mines2DVector[coords.col][coords.row] == 'X')
    {
        this->setButtonIcon(
                    this->getButtonFromCoords(coords),
                    button_mine_hit
                    );
    }
}

// automatically reveal all connected buttons, as long as they have no neighbour mines:
void Field::autoReveal(
        const Common::Coords& coords,
        QVector<quint16>& poolVector,
        bool aiReveal
        )
{
    // create vector holding unrevealed neighbours:
    QVector<Common::Coords> neighboursUnrevealedVector;
    neighboursUnrevealedVector = this->findNeighbours(
                this->field2DVector,
                coords,
                ' '
                );

    for (quint16 i = 0; i < neighboursUnrevealedVector.size(); ++i)
    {
        QVector<Common::Coords> neighboursMinesVector;
        neighboursMinesVector = this->findNeighbours(
                    this->mines2DVector,
                    neighboursUnrevealedVector.at(i),
                    'X'
                    );
        if (
                std::find(
                    poolVector.begin(),
                    poolVector.end(),
                    Common::CoordsToInt(
                        neighboursUnrevealedVector.at(i),
                        this->cols
                        )
                    ) == poolVector.end()
                )
        {
            if (this->mines2DVector[neighboursUnrevealedVector.at(i).col][neighboursUnrevealedVector.at(i).row] != 'X')
            {
                QVector<Common::Coords> neighboursZerosVector;
                neighboursZerosVector = this->findNeighbours(
                            this->field2DVector,
                            neighboursUnrevealedVector.at(i),
                            '0'
                            );
                if (neighboursZerosVector.size() == 0)
                {
                    continue;
                }
                else
                {
                    if (aiReveal == true && this->isNewGameRequested == false)
                    {
                        Common::sleep(5);
                    }
                    poolVector.push_back(Common::CoordsToInt(neighboursUnrevealedVector.at(i), this->cols));
                    this->setNumber(
                                neighboursUnrevealedVector.at(i),
                                neighboursMinesVector.size()
                                );
                    this->printNumber(
                                neighboursUnrevealedVector.at(i),
                                neighboursMinesVector.size()
                                );
                    --this->countUnrevealed;
                }
            }
            if (neighboursMinesVector.size() == 0)
            {
                this->autoReveal(
                            neighboursUnrevealedVector.at(i),
                            poolVector,
                            aiReveal
                            );
            }
        }
    }
}

void Field::flagAutoReveal(
        const Common::Coords& coords,
        bool hasCheated,
        bool aiReveal
        )
{
    // create a new vector of surrounding flags:
    QVector<Common::Coords> neighboursFlagsVector = this->findNeighbours(
                this->field2DVector,
                coords,
                'F'
                );

    // if player has placed some flags around coords:
    if (neighboursFlagsVector.size() != 0)
    {
        // create a new vector of surrounding mines:
        QVector<Common::Coords> neighboursMinesVector = this->findNeighbours(
                    this->mines2DVector,
                    coords,
                    'X'
                    );

        // only proceed if the flag number matches the number of actual surrounding mines:
        if (neighboursMinesVector.size() == neighboursFlagsVector.size())
        {
            // create a new vector of surrounding unrevealed squares:
            QVector<Common::Coords> neighboursUnrevealedVector = this->findNeighbours(
                        this->field2DVector,
                        coords,
                        ' '
                        );

            // create a new empty vector for missed mines:
            QVector<Common::Coords> missedMinesVector;

            // for each unrevealed neighbour of coords check if the player has missed a mine
            // and add this mines position to flagRevealMissedMinesVector:
            for (quint16 i = 0; i < neighboursUnrevealedVector.size(); ++i)
            {
                if (this->mines2DVector[neighboursUnrevealedVector.at(i).col][neighboursUnrevealedVector.at(i).row] == 'X')
                {
                    missedMinesVector.push_back(neighboursUnrevealedVector.at(i));
                }
            }
            // if there are missed mines, reveal the mines2DVector - player has lost:
            if (missedMinesVector.size() != 0)
            {
                for (quint16 i = 0; i < missedMinesVector.size(); ++i)
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
                QVector<quint16> poolVector;
                // for each unrevealed neighbour of coords, print the number of surrounding mines:
                for (quint16 i = 0; i < neighboursUnrevealedVector.size(); ++i)
                {
                    Common::Coords coordsTemp;
                    coordsTemp.col = neighboursUnrevealedVector.at(i).col;
                    coordsTemp.row = neighboursUnrevealedVector.at(i).row;
                    QVector<Common::Coords> neighboursMinesOfNeighboursUnrevealedVector = this->findNeighbours(
                                this->mines2DVector,
                                coordsTemp,
                                'X'
                                );

                    if (
                            std::find(
                                poolVector.begin(),
                                poolVector.end(),
                                Common::CoordsToInt(coordsTemp, this->cols)
                                ) == poolVector.end()
                            )
                    {
                        if (this->flagsCount + this->countUnrevealed == this->mines)
                        {
                            Common::Coords dummyCoords;
                            this->gameOver(dummyCoords, false);
                        }
                        else if (neighboursMinesOfNeighboursUnrevealedVector.size() == 0)
                        {
                            this->setNumber(
                                        coordsTemp,
                                        0
                                        );
                            this->printNumber(
                                        coordsTemp,
                                        0
                                        );
                            poolVector.push_back(Common::CoordsToInt(coordsTemp, this->cols));
                            --this->countUnrevealed;
                            this->autoReveal(
                                        coordsTemp,
                                        poolVector,
                                        aiReveal
                                        );
                        }
                        else
                        {
                            this->setNumber(
                                        coordsTemp,
                                        neighboursMinesOfNeighboursUnrevealedVector.size()
                                        );
                            this->printNumber(
                                        coordsTemp,
                                        neighboursMinesOfNeighboursUnrevealedVector.size()
                                        );
                            poolVector.push_back(Common::CoordsToInt(coordsTemp, this->cols));
                            --this->countUnrevealed;
                        }
                    }
                }
            }
            if (
                    neighboursUnrevealedVector.size() != 0
                    && this->flagsCount + this->countUnrevealed != this->mines
                    && hasCheated == false
                    )
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
//    qDebug() << QString::number(structTemp->coords.col) << ", " << QString::number(structTemp->coords.row);
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
    this->setButtonIcon(
                this->getButtonFromCoords(coords),
                button_flag
                );
    this->flagsCount++;
    this->minesLeft--;
    this->countUnrevealed--;
    emit this->minesleft_changed_signal(this->minesLeft);
}

void Field::is_solver_running_slot(bool running)
{
    this->isSolverRunning = running;
}
