#include <QDebug>

#include "mouseinput.h"
#include "sleep.h"

MouseInput::MouseInput(Field *field_, const Button::ButtonIconsStruct& buttonIcons_)
{
    this->field = field_;
    this->buttonIcons = buttonIcons_;
}

MouseInput::~MouseInput() {}

// methods:
Common::Coords MouseInput::getCoordsFromRelativeMousePosition()
{
    Common::Coords returnCoords;
    returnCoords = this->pressedButtonCoords;

    assert (field->buttonSize != 0);

    if (this->currentMousePosition.x() < 0)
    {
        returnCoords.col = this->pressedButtonCoords.col - (((this->currentMousePosition.x() * -1) / field->buttonSize) + 1);
    }
    else if (this->currentMousePosition.x() >= field->buttonSize)
    {
        returnCoords.col = this->pressedButtonCoords.col + (this->currentMousePosition.x() / field->buttonSize);
    }
    if (this->currentMousePosition.y() < 0)
    {
        returnCoords.row = this->pressedButtonCoords.row - (((this->currentMousePosition.y() * -1) / field->buttonSize) + 1);
    }
    else if (this->currentMousePosition.y() >= field->buttonSize)
    {
        returnCoords.row = this->pressedButtonCoords.row + (this->currentMousePosition.y() / field->buttonSize);
    }

    return returnCoords;
}
void MouseInput::leftPressed()
{
    if (field->field2DVector[this->pressedButtonCoords.col][this->pressedButtonCoords.row] == ' ')
    {
        if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
        {
            field->setButtonIcon(
                        field->getButtonFromCoords(this->pressedButtonCoords),
                        this->buttonIcons.button_pressed
                        );
        }
    }
}
void MouseInput::leftAndRightPressed()
{
    Common::Coords newCoords = this->getCoordsFromRelativeMousePosition();

    if (
            newCoords.col > 0
            && newCoords.col <= field->cols
            && newCoords.row > 0
            && newCoords.row <= field->rows
            )
    {
        this->leftAndRightPressedNeighboursCoveredVector = field->findNeighbours(
                    field->field2DVector,
                    newCoords,
                    ' '
                    );
        for (quint16 i = 0; i < leftAndRightPressedNeighboursCoveredVector.size(); i++)
        {
            if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
            {
                Button *button = field->getButtonFromCoords(leftAndRightPressedNeighboursCoveredVector[i]);
                field->setButtonIcon(
                            button,
                            this->buttonIcons.button_pressed
                            );
            }
        }
        if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
        {
            if (field->field2DVector[newCoords.col][newCoords.row] == ' ')
            {
                    field->setButtonIcon(
                                field->getButtonFromCoords(newCoords),
                                this->buttonIcons.button_pressed
                                );
            }
        }
    }
}
void MouseInput::leftReleased()
{
    Common::Coords newCoords = this->getCoordsFromRelativeMousePosition();

    if (
            newCoords.col > 0
            && newCoords.col <= field->cols
            && newCoords.row > 0
            && newCoords.row <= field->rows
            )
    {
        if (field->field2DVector[newCoords.col][newCoords.row] == ' ')
        {
            QVector<Common::Coords> neighboursMinesVector;

            // fill mines2DVector with mines only once after users first guess:
            if (field->firstTurn == true)
            {
                field->fillMines2DVector(newCoords);
                emit field->game_started_signal();
            }

            // if user hit a mine, reveal the game field - game is lost:
            if (field->mines2DVector[newCoords.col][newCoords.row] == 'X')
            {
                field->gameOver(newCoords, true);
            }

            else
            {
                // reveal the players choice and place the number of surrounding mines in it:
                neighboursMinesVector = field->findNeighbours(
                            field->mines2DVector,
                            newCoords,
                            'X'
                            );
                field->setNumber(
                            newCoords,
                            neighboursMinesVector.size()
                            );
                field->printNumber(
                            newCoords,
                            neighboursMinesVector.size()
                            );
                --field->countUnrevealed;

                // check if player has won:
                if (field->flagsCount + field->countUnrevealed == field->mines)
                {
                    Common::Coords dummyCoords;
                    field->gameOver(dummyCoords, false);
                }
            }
            if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
            {
                // automatically reveal all neighbours of squares with no neighbour mines:
                QVector<quint16> poolVector;
                field->autoReveal(
                            newCoords,
                            poolVector,
                            false
                            );
                field->firstTurn = false;

                // check if player has won:
                if (field->flagsCount + field->countUnrevealed == field->mines)
                {
                    Common::Coords dummyCoords;
                    field->gameOver(dummyCoords, false);
                }
            }

            if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
            {
                if (newCoords.col != this->pressedButtonCoords.col || newCoords.row != this->pressedButtonCoords.row)
                {
                    if (field->field2DVector[this->pressedButtonCoords.col][this->pressedButtonCoords.row] == ' ')
                    {
                        field->setButtonIcon(
                                    field->getButtonFromCoords(this->pressedButtonCoords),
                                    this->buttonIcons.button_unrevealed
                                    );
                    }
                }
            }
            emit this->smiley_surprised_queued_signal();
        }
        this->currentMousePosition.setX(0);
        this->currentMousePosition.setY(0);
    }
}
void MouseInput::rightReleased()
{
    Common::Coords newCoords = this->getCoordsFromRelativeMousePosition();
    Button *button;
    button = field->getButtonFromCoords(newCoords);

    if (
            newCoords.col > 0
            && newCoords.col <= field->cols
            && newCoords.row > 0
            && newCoords.row <= field->rows
            )
    {
        if (field->field2DVector[newCoords.col][newCoords.row] == ' ')
        {
            field->setButtonIcon(
                        button,
                        this->buttonIcons.button_flag
                        );
            field->field2DVector[newCoords.col][newCoords.row] = 'F';
            field->flagsCount++;
            field->minesLeft--;
            field->countUnrevealed--;
        }
        else if (field->isNumber(newCoords) == false)
        {
            field->setButtonIcon(
                        button,
                        this->buttonIcons.button_unrevealed
                        );
            field->field2DVector[newCoords.col][newCoords.row] = ' ';
            field->flagsCount--;
            field->minesLeft++;
            field->countUnrevealed++;
        }
        emit field->minesleft_changed_signal(field->minesLeft);
    }
    this->currentMousePosition.setX(0);
    this->currentMousePosition.setY(0);
}
void MouseInput::leftAndRightReleased()
{
    Common::Coords newCoords = this->getCoordsFromRelativeMousePosition();

    if (
            newCoords.col > 0
            && newCoords.col <= field->cols
            && newCoords.row > 0
            && newCoords.row <= field->rows
            )
    {
        if (
                newCoords.col == this->lastButtonCoords.col
                && newCoords.row == this->lastButtonCoords.row
                )
        {
            if (field->isNumber(newCoords) == true)
            {
                field->flagAutoReveal(
                            newCoords,
                            false,
                            false
                            );
            }
        }

        // check if player has won:
        if (field->flagsCount + field->countUnrevealed == field->mines)
        {
            Common::Coords dummyCoords;
            field->gameOver(dummyCoords, false);
        }
        if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
        {
            if (field->field2DVector[newCoords.col][newCoords.row] == ' ')
            {
                    field->setButtonIcon(
                                field->getButtonFromCoords(newCoords),
                                this->buttonIcons.button_unrevealed
                                );
            }
        }
        if (
                this->lastButtonCoords.col > 0
                && this->lastButtonCoords.col <= field->cols
                && this->lastButtonCoords.row > 0
                && this->lastButtonCoords.row <= field->rows
                )
        {
            for (quint16 i = 0; i < this->leftAndRightPressedNeighboursCoveredVector.size(); i++)
            {
                if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
                {
                    if (field->field2DVector[this->leftAndRightPressedNeighboursCoveredVector[i].col][this->leftAndRightPressedNeighboursCoveredVector[i].row] == ' ')
                    {
                        field->setButtonIcon(
                                    field->getButtonFromCoords(this->leftAndRightPressedNeighboursCoveredVector[i]),
                                    this->buttonIcons.button_unrevealed
                                    );
                    }
                }
            }
            if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
            {
                if (field->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
                {
                        field->setButtonIcon(
                                    field->getButtonFromCoords(this->lastButtonCoords),
                                    this->buttonIcons.button_unrevealed
                                    );
                }
            }
        }
    }
    this->currentMousePosition.setX(0);
    this->currentMousePosition.setY(0);
}
void MouseInput::leftPressedAndMoved()
{
    Common::Coords newButtonCoords = this->getCoordsFromRelativeMousePosition();

    if (this->lastButtonCoords.col == 0)
    {
        this->lastButtonCoords.col = this->pressedButtonCoords.col;
    }
    if (this->lastButtonCoords.row == 0)
    {
        this->lastButtonCoords.row = this->pressedButtonCoords.row;
    }

    if (
            newButtonCoords.col < 1
            || newButtonCoords.col > field->cols
            || newButtonCoords.row < 1
            || newButtonCoords.row > field->rows
            )
    {
//        emit this->print_debug_signal(QString::number(lastButtonCoords.col) + "," + QString::number(lastButtonCoords.row));
        if (
                this->lastButtonCoords.col > 0
                && this->lastButtonCoords.col <= field->cols
                && this->lastButtonCoords.row > 0
                && this->lastButtonCoords.row <= field->rows
                )
        {
            if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
            {
                if (field->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
                {
                    field->setButtonIcon(
                                field->getButtonFromCoords(this->lastButtonCoords),
                                this->buttonIcons.button_unrevealed
                                );
                }
            }
        }

        if (newButtonCoords.col < 1)
        {
            this->lastButtonCoords.col = 0;
        }
        else if (newButtonCoords.col > field->cols)
        {
            this->lastButtonCoords.col = field->cols + 1;
        }
        if (newButtonCoords.row < 1)
        {
            this->lastButtonCoords.row = 0;
        }
        else if (newButtonCoords.row > field->rows)
        {
            this->lastButtonCoords.row = field->rows + 1;
        }
    }
    else if (
             this->lastButtonCoords.col >= 0
             && this->lastButtonCoords.col <= field->cols + 1
             && this->lastButtonCoords.row >= 0
             && this->lastButtonCoords.row <= field->rows + 1
             )
    {
        if (this->lastButtonCoords.col != newButtonCoords.col || this->lastButtonCoords.row != newButtonCoords.row)
        {
            if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
            {
                if (field->field2DVector[this->pressedButtonCoords.col][this->pressedButtonCoords.row] == ' ')
                {
                    field->setButtonIcon(
                                field->getButtonFromCoords(this->pressedButtonCoords),
                                this->buttonIcons.button_unrevealed
                                );
                }
            }
            if (
                    this->lastButtonCoords.col > 0
                    && this->lastButtonCoords.col <= field->cols
                    && this->lastButtonCoords.row > 0
                    && this->lastButtonCoords.row <= field->rows
                    )
            {
                if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
                {
                    if (field->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
                    {
                        field->setButtonIcon(
                                    field->getButtonFromCoords(this->lastButtonCoords),
                                    this->buttonIcons.button_unrevealed
                                    );
                    }
                }
            }
            if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
            {
                if (field->field2DVector[newButtonCoords.col][newButtonCoords.row] == ' ')
                {
                    field->setButtonIcon(
                                field->getButtonFromCoords(newButtonCoords),
                                this->buttonIcons.button_pressed
                                );
                }
            }
            this->lastButtonCoords.col = newButtonCoords.col;
            this->lastButtonCoords.row = newButtonCoords.row;
        }
    }
}
void MouseInput::leftAndRightPressedAndMoved()
{
    Common::Coords newButtonCoords = this->getCoordsFromRelativeMousePosition();

    if (this->lastButtonCoords.col == 0)
    {
        this->lastButtonCoords.col = this->pressedButtonCoords.col;
    }
    if (this->lastButtonCoords.row == 0)
    {
        this->lastButtonCoords.row = this->pressedButtonCoords.row;
    }

    if (
            newButtonCoords.col < 1
            || newButtonCoords.col > field->cols
            || newButtonCoords.row < 1
            || newButtonCoords.row > field->rows
            )
    {
        if (
                this->lastButtonCoords.col > 0
                && this->lastButtonCoords.col <= field->cols
                && this->lastButtonCoords.row > 0
                && this->lastButtonCoords.row <= field->rows
                )
        {
            for (quint16 i = 0; i < this->leftAndRightPressedNeighboursCoveredVector.size(); i++)
            {
                if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
                {
                    field->setButtonIcon(
                                field->getButtonFromCoords(this->leftAndRightPressedNeighboursCoveredVector[i]),
                                this->buttonIcons.button_unrevealed
                                );
                }
            }
    //                    emit this->print_debug_signal(QString::number(lastButtonCoords.col) + "," + QString::number(lastButtonCoords.row));

            if (field->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
            {
                if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
                {
                    field->setButtonIcon(
                                field->getButtonFromCoords(this->lastButtonCoords),
                                this->buttonIcons.button_unrevealed
                                );
                }
            }
        }

        if (newButtonCoords.col < 1)
        {
            this->lastButtonCoords.col = 0;
        }
        else if (newButtonCoords.col > field->cols)
        {
            this->lastButtonCoords.col = field->cols + 1;
        }
        if (newButtonCoords.row < 1)
        {
            this->lastButtonCoords.row = 0;
        }
        else if (newButtonCoords.row > field->rows)
        {
            this->lastButtonCoords.row = field->rows + 1;
        }
    }
    else if (
             this->lastButtonCoords.col >= 0
             && this->lastButtonCoords.col <= field->cols + 1
             && this->lastButtonCoords.row >= 0
             && this->lastButtonCoords.row <= field->rows + 1
             )
    {
        if (this->lastButtonCoords.col != newButtonCoords.col || this->lastButtonCoords.row != newButtonCoords.row)
        {
            for (quint16 i = 0; i < this->leftAndRightPressedNeighboursCoveredVector.size(); i++)
            {
                if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
                {
                    if (field->field2DVector[this->leftAndRightPressedNeighboursCoveredVector[i].col][this->leftAndRightPressedNeighboursCoveredVector[i].row] == ' ')
                    {
                        field->setButtonIcon(
                                    field->getButtonFromCoords(this->leftAndRightPressedNeighboursCoveredVector[i]),
                                    this->buttonIcons.button_unrevealed
                                    );
                    }
                }
            }
            if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
            {
                if (field->field2DVector[newButtonCoords.col][newButtonCoords.row] == ' ')
                {
                    field->setButtonIcon(
                                field->getButtonFromCoords(newButtonCoords),
                                this->buttonIcons.button_pressed
                                );
                }
            }
            if (
                    this->lastButtonCoords.col > 0
                    && this->lastButtonCoords.col <= field->cols
                    && this->lastButtonCoords.row > 0
                    && this->lastButtonCoords.row <= field->rows
                    )
            {
                if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
                {
                    if (field->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
                    {
                        field->setButtonIcon(
                                    field->getButtonFromCoords(this->lastButtonCoords),
                                    this->buttonIcons.button_unrevealed
                                    );
                    }
                }
            }
            this->leftAndRightPressedNeighboursCoveredVector = field->findNeighbours(
                        field->field2DVector,
                        newButtonCoords,
                        ' '
                        );
            for (quint16 i = 0; i < leftAndRightPressedNeighboursCoveredVector.size(); i++)
            {
                if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
                {
                    if (field->field2DVector[leftAndRightPressedNeighboursCoveredVector[i].col][leftAndRightPressedNeighboursCoveredVector[i].row] == ' ')
                    {
                        Button *button = field->getButtonFromCoords(leftAndRightPressedNeighboursCoveredVector[i]);
                        field->setButtonIcon(
                                    button,
                                    this->buttonIcons.button_pressed
                                    );
                    }
                }
            }
            this->lastButtonCoords.col = newButtonCoords.col;
            this->lastButtonCoords.row = newButtonCoords.row;
        }
    }
}

// slots:
void MouseInput::connect_button_slot(Button *button)
{
    connect(button, &Button::left_pressed_signal, this, &MouseInput::left_pressed_slot);
    connect(button, &Button::right_pressed_signal, this, &MouseInput::right_pressed_slot);
    connect(button, &Button::left_and_right_pressed_signal, this, &MouseInput::left_and_right_pressed_slot);
    connect(button, &Button::left_released_signal, this, &MouseInput::left_released_slot);
    connect(button, &Button::right_released_signal, this, &MouseInput::right_released_slot);
    connect(button, &Button::left_and_right_released_signal, this, &MouseInput::left_and_right_released_slot);
    connect(button, &Button::left_pressed_and_moved_signal, this, &MouseInput::left_pressed_and_moved_slot);
    connect(button, &Button::right_pressed_and_moved_signal, this, &MouseInput::right_pressed_and_moved_slot);
    connect(button, &Button::left_and_right_pressed_and_moved_signal, this, &MouseInput::left_and_right_pressed_and_moved_slot);
    connect(button, &Button::print_debug_signal, &this->common, &Common::print_debug_slot);
}
void MouseInput::left_pressed_slot(QMouseEvent* e)
{
    if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
    {
        Button* button = static_cast<Button*>(sender());
        this->pressedButtonCoords = field->getCoordsFromButton(button);
        this->currentMousePosition = e->pos();
        this->lastButtonCoords = this->getCoordsFromRelativeMousePosition();
        this->leftPressed();
    }
}
void MouseInput::right_pressed_slot(QMouseEvent* e)
{
    if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
    {
        Button* button = static_cast<Button*>(sender());
        this->pressedButtonCoords = field->getCoordsFromButton(button);
        this->currentMousePosition = e->pos();
        this->lastButtonCoords = this->getCoordsFromRelativeMousePosition();
    }
}
void MouseInput::left_and_right_pressed_slot(QMouseEvent* e)
{
    if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
    {
        Button* button = static_cast<Button*>(sender());
        this->pressedButtonCoords = field->getCoordsFromButton(button);
        this->currentMousePosition = e->pos();
        this->lastButtonCoords = this->getCoordsFromRelativeMousePosition();
        this->leftAndRightPressed();
    }
}
void MouseInput::left_released_slot()
{
    if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
    {
        this->leftReleased();
    }
}
void MouseInput::right_released_slot()
{
    if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
    {
        this->rightReleased();
    }
}
void MouseInput::left_and_right_released_slot()
{
    if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
    {
        this->leftAndRightReleased();
    }
}
void MouseInput::left_pressed_and_moved_slot(QMouseEvent* e)
{
    if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
    {
        this->currentMousePosition = e->pos();
//        emit this->print_debug_signal(QString::number(this->currentMousePosition.x()) + "," + QString::number(this->currentMousePosition.y()));
        this->leftPressedAndMoved();
    }
}
void MouseInput::right_pressed_and_moved_slot(QMouseEvent* e)
{
    if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
    {
        this->currentMousePosition = e->pos();
//        emit this->print_debug_signal(QString::number(this->currentMousePosition.x()) + "," + QString::number(this->currentMousePosition.y()));
    }
}
void MouseInput::left_and_right_pressed_and_moved_slot(QMouseEvent* e)
{
    if (this->isGameOver == false && this->isSolverRunning == false && this->isNewGameRequested == false)
    {
        this->currentMousePosition = e->pos();
//            emit this->print_debug_signal(QString::number(this->currentMousePosition.x()) + "," + QString::number(this->currentMousePosition.y()));

        if (this->lastButtonCoords.col < 0) this->lastButtonCoords.col = 0;
        else if (this->lastButtonCoords.col > field->cols + 1) this->lastButtonCoords.col = field->cols + 1;
        if (this->lastButtonCoords.row < 0) this->lastButtonCoords.row = 0;
        else if (this->lastButtonCoords.row > field->rows + 1) this->lastButtonCoords.row = field->rows + 1;

        this->leftAndRightPressedAndMoved();
    }
}
void MouseInput::is_solver_running_slot(bool running)
{
    this->isSolverRunning = running;
}
