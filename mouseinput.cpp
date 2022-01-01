#include <QDebug>

#include "mouseinput.h"

MouseInput::MouseInput(Field *field_)
{
    this->field = field_;
    this->isLeftAndRightPressed = false;
}

MouseInput::~MouseInput()
{}

Common::Coords MouseInput::getCoordsFromMousePosition()
{
    Common::Coords returnCoords = this->pressedButtonCoords;

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
    this->currentMousePosition.setX(0);
    this->currentMousePosition.setY(0);
    Button *button = qobject_cast<Button*>(sender());
    this->pressedButtonCoords = field->getCoordsFromButton(button);

    Common::Coords currentMouseCoords = this->getCoordsFromMousePosition();
    if (field->field2DVector[this->pressedButtonCoords.col][this->pressedButtonCoords.row] == ' '
            && this->pressedButtonCoords.col == currentMouseCoords.col
            && this->pressedButtonCoords.row == currentMouseCoords.row)
    {
        field->setButtonIcon(field->getButtonFromCoords(this->pressedButtonCoords), "pressed");
    }
}

void MouseInput::leftAndRightPressed(const Common::Coords& coords)
{
    this->leftAndRightPressedNeighboursCoveredVector.clear();
    this->leftAndRightPressedNeighboursCoveredVector = field->findNeighbours(field->field2DVector, coords, ' ');
    for (qint32 i = 0; i < leftAndRightPressedNeighboursCoveredVector.size(); i++)
    {
        Button *button = field->getButtonFromCoords(leftAndRightPressedNeighboursCoveredVector[i]);
        field->setButtonIcon(button, "revealed");
    }
}

void MouseInput::leftPressedAndMoved(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton)
    {
        this->currentMousePosition = e->pos();
        Common::Coords newButtonCoords = this->getCoordsFromMousePosition();

        if (this->lastButtonCoords.col == 0)
        {
            this->lastButtonCoords.col = this->pressedButtonCoords.col;
        }
        if (this->lastButtonCoords.row == 0)
        {
            this->lastButtonCoords.row = this->pressedButtonCoords.row;
        }

//        qDebug() << "last:" << QString::number(this->lastButtonCoords.col) << "," << QString::number(this->lastButtonCoords.row) <<
//                    "new:"  << QString::number(newButtonCoords.col) << "," << QString::number(newButtonCoords.row);

        if (newButtonCoords.col < 1
                || newButtonCoords.col > field->cols
                || newButtonCoords.row < 1
                || newButtonCoords.row > field->rows)
        {
            if (field->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
            {
                field->setButtonIcon(field->getButtonFromCoords(this->lastButtonCoords), "unrevealed");
            }

            if (newButtonCoords.col < 1)
            {
                this->lastButtonCoords.col = 0;
            }
            else if (newButtonCoords.col > field->cols)
            {
                this->lastButtonCoords.col = field->cols - 1;
            }
            if (newButtonCoords.row < 1)
            {
                this->lastButtonCoords.row = 0;
            }
            else if (newButtonCoords.row > field->rows)
            {
                this->lastButtonCoords.row = field->rows - 1;
            }
        }
        else if (this->lastButtonCoords.col >= 1
                 && this->lastButtonCoords.row >= 1
                 && this->lastButtonCoords.col <= field->cols
                 && this->lastButtonCoords.row <= field->rows)
        {
            if (this->lastButtonCoords.col != newButtonCoords.col || this->lastButtonCoords.row != newButtonCoords.row)
            {
                if (field->field2DVector[this->pressedButtonCoords.col][this->pressedButtonCoords.row] == ' ')
                {
                    field->setButtonIcon(field->getButtonFromCoords(this->pressedButtonCoords), "unrevealed");
                }
                if (field->field2DVector[this->lastButtonCoords.col][this->lastButtonCoords.row] == ' ')
                {
                    field->setButtonIcon(field->getButtonFromCoords(this->lastButtonCoords), "unrevealed");
                }
                if (field->field2DVector[newButtonCoords.col][newButtonCoords.row] == ' ')
                {
                    field->setButtonIcon(field->getButtonFromCoords(newButtonCoords), "pressed");
                }
                this->lastButtonCoords.col = newButtonCoords.col;
                this->lastButtonCoords.row = newButtonCoords.row;
            }
        }
    }
}

// handle left clicking on a button:
void MouseInput::leftReleased()
{
    Button *button = qobject_cast<Button*>(sender());
    Common::Coords coordsTemp = field->getCoordsFromButton(button);
    Common::Coords currentMouseCoords = this->getCoordsFromMousePosition();
    if (currentMouseCoords.col != this->pressedButtonCoords.col
            || currentMouseCoords.row != this->pressedButtonCoords.row)
    {
        coordsTemp = this->lastButtonCoords;
    }
    if (field->field2DVector[coordsTemp.col][coordsTemp.row] == ' ')
    {
        if (currentMouseCoords.col >= 1
                && currentMouseCoords.col <= field->cols
                && currentMouseCoords.row >= 1
                && currentMouseCoords.row <= field->rows)
        {
            QVector<Common::Coords> neighboursMinesVector;

            // fill mines2DVector with mines only once after users first guess:
            if (field->firstTurn)
            {
                field->fillMines2DVector(coordsTemp);
                emit field->game_started_signal();
            }

            // if user hit a mine, reveal the game field - game is lost:
            if (field->mines2DVector[coordsTemp.col][coordsTemp.row] == 'X')
            {
                field->gameOver(coordsTemp, true);
            }

            else
            {
                // reveal the players choice and place the number of surrounding mines in it:
                neighboursMinesVector = field->findNeighbours(field->mines2DVector, coordsTemp, 'X');
                field->setNumber(coordsTemp, neighboursMinesVector.size());
                field->printNumber(coordsTemp, neighboursMinesVector.size());
                --field->countUnrevealed;

                // check if player has won:
                if (field->flagsCount + field->countUnrevealed == field->mines)
                {
                    Common::Coords dummyCoords;
                    field->gameOver(dummyCoords, false);
                }
            }
            if (field->isGameOver != true)
            {
                // automatically reveal all neighbours of squares with no neighbour mines:
                QVector<qint32> poolVector;
                field->autoReveal(coordsTemp, poolVector, false);
                field->firstTurn = false;

                // check if player has won:
                if (field->flagsCount + field->countUnrevealed == field->mines)
                {
                    Common::Coords dummyCoords;
                    field->gameOver(dummyCoords, false);
                }
            }
            emit field->smiley_surprised_signal();
        }
    }
    this->lastButtonCoords.col = 0;
    this->lastButtonCoords.row = 0;
}

// place and remove flags with right click:
void MouseInput::rightReleased(const Common::Coords& coords)
{
    Button *button;
    Common::Coords coordsTemp;
    if (this->lastButtonCoords.col != 0 || this->lastButtonCoords.row != 0)
    {
        coordsTemp = this->getCoordsFromMousePosition();
    }
    else
    {
        coordsTemp = coords;
    }
    button = field->getButtonFromCoords(coordsTemp);

    if (field->field2DVector[coordsTemp.col][coordsTemp.row] == ' ')
    {
        field->setButtonIcon(button, "flag");
        field->field2DVector[coordsTemp.col][coordsTemp.row] = 'F';
        field->flagsCount++;
        field->minesLeft--;
        field->countUnrevealed--;
    }
    else if (! field->isNumber(coords))
    {
        field->setButtonIcon(button, "unrevealed");
        field->field2DVector[coordsTemp.col][coordsTemp.row] = ' ';
        field->flagsCount--;
        field->minesLeft++;
        field->countUnrevealed++;
    }
    emit field->minesleft_changed_signal(field->minesLeft);

    this->lastButtonCoords.col = 0;
    this->lastButtonCoords.row = 0;
}

// auto-reveal safe buttons with no neighbour mines by double clicking on a number
// (flags around the numbered button must be placed right):
void MouseInput::leftAndRightReleased(const Common::Coords& coords)
{
    if (field->isNumber(coords))
    {
        field->flagAutoReveal(coords, false, false);
    }

    // check if player has won:
    if (field->flagsCount + field->countUnrevealed == field->mines)
    {
        Common::Coords dummyCoords;
        field->gameOver(dummyCoords, false);
    }
    this->lastButtonCoords.col = 0;
    this->lastButtonCoords.row = 0;
}

void MouseInput::connect_button_slot(Button *button)
{
    connect(button, &Button::left_pressed_signal, this, &MouseInput::left_pressed_slot);
    connect(button, &Button::right_pressed_signal, this, &MouseInput::right_pressed_slot);
    connect(button, &Button::left_released_signal, this, &MouseInput::left_released_slot);
    connect(button, &Button::right_released_signal, this, &MouseInput::right_released_slot);
    connect(button, &Button::left_and_right_pressed_signal, this, &MouseInput::left_and_right_pressed_slot);
    connect(button, &Button::left_and_right_released_signal, this, &MouseInput::left_and_right_released_slot);
    connect(button, &Button::left_pressed_and_moved_signal, this, &MouseInput::left_pressed_and_moved_slot);
    connect(button, &Button::left_and_right_pressed_and_moved_signal, this, &MouseInput::left_and_right_pressed_and_moved_slot);
}

void MouseInput::left_pressed_slot()
{
    if (field->isGameOver != true && field->isSolverRunning != true)
    {
//        emit this->print_debug_signal("left pressed");
        this->leftPressed();
    }
}

void MouseInput::right_pressed_slot()
{
    if (field->isGameOver != true && field->isSolverRunning != true)
    {
//        emit this->print_debug_signal("right pressed");
    }
}

void MouseInput::left_released_slot()
{
    if (field->isGameOver != true && field->isSolverRunning != true)
    {
        if (! this->isLeftAndRightPressed)
        {
//            emit this->print_debug_signal("left released");
            this->leftReleased();
        }
    }
    this->isLeftAndRightPressed = false;
}

void MouseInput::right_released_slot()
{
    if (field->isGameOver != true && field->isSolverRunning != true)
    {
        if (! this->isLeftAndRightPressed)
        {
            Button *button = qobject_cast<Button*>(sender());
            Common::Coords coords = field->getCoordsFromButton(button);
//            emit this->print_debug_signal("right released");
            this->rightReleased(coords);
        }
        this->isLeftAndRightPressed = false;
    }
}

void MouseInput::left_and_right_pressed_slot()
{
    if (field->isGameOver != true && field->isSolverRunning != true)
    {
        Button *button = qobject_cast<Button*>(sender());
        Common::Coords coords = field->getCoordsFromButton(button);
        if (field->isNumber(coords))
        {
//            emit this->print_debug_signal("left and right pressed");
            this->pressedButtonCoords = field->getCoordsFromButton(button);
            this->isLeftAndRightPressed = true;
            this->leftAndRightPressed(coords);
        }
    }
}

void MouseInput::left_and_right_released_slot()
{
    if (field->isGameOver != true && field->isSolverRunning != true)
    {
        Button *button = qobject_cast<Button*>(sender());
        Common::Coords coords = field->getCoordsFromButton(button);
        if (field->isNumber(coords))
        {
            Button *button = qobject_cast<Button*>(sender());
            Common::Coords coords = field->getCoordsFromButton(button);
            Common::Coords mousePosition = this->getCoordsFromMousePosition();
            if (mousePosition.col == this->pressedButtonCoords.col && mousePosition.row == this->pressedButtonCoords.row)
            {
//                emit this->print_debug_signal("left and right released");
                this->leftAndRightReleased(coords);
            }
            for (qint32 i = 0; i < this->leftAndRightPressedNeighboursCoveredVector.size(); i++)
            {
                if (! field->isNumber(this->leftAndRightPressedNeighboursCoveredVector[i])
                        && field->isGameOver != true)
                {
                    Button *buttonTemp = field->getButtonFromCoords(leftAndRightPressedNeighboursCoveredVector[i]);
                    field->setButtonIcon(buttonTemp, "unrevealed");
                }
            }
        }
        this->isLeftAndRightPressed = false;
    }
}

void MouseInput::left_pressed_and_moved_slot(QMouseEvent *mouseEvent)
{
    if (field->isGameOver != true && field->isSolverRunning != true)
    {
        if (! this->isLeftAndRightPressed)
        {
            this->leftPressedAndMoved(mouseEvent);
        }
    }
}

void MouseInput::left_and_right_pressed_and_moved_slot(QMouseEvent *mouseEvent)
{
    if (field->isGameOver != true && field->isSolverRunning != true)
    {
//        emit this->print_debug_signal("left and right pressed and moved");
        this->currentMousePosition = mouseEvent->pos();
    }
}
