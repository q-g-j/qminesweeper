#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include <QMouseEvent>
#include <QObject>

#include "button.h"
#include "common.h"
#include "field.h"
#include "stylesheet.h"

class MouseInput : public QObject
{
    Q_OBJECT
private:
    Field *field;
    Stylesheet stylesheet;
    Common common;

    bool isLeftAndRightPressed = false;
    bool isSolverRunning = false;

    QPoint currentMousePosition;
    Common::Coords pressedButtonCoords;
    Common::Coords lastButtonCoords;
    QVector<Common::Coords> leftAndRightPressedNeighboursCoveredVector;

    void leftPressed();
    void rightPressed();
    void leftAndRightPressed();

    void leftReleased();
    void rightReleased();
    void leftAndRightReleased();

    void leftPressedAndMoved();
    void leftAndRightPressedAndMoved();

    Common::Coords getCoordsFromRelativeMousePosition();
public:
    MouseInput(Field*);
    ~MouseInput();

    bool isGameOver = false;
    bool isNewGameRequested = false;
public slots:
    void connect_button_slot(Button*);

    void left_pressed_slot(QMouseEvent*);
    void right_pressed_slot(QMouseEvent*);
    void left_and_right_pressed_slot(QMouseEvent*);

    void left_released_slot();
    void right_released_slot();
    void left_and_right_released_slot();

    void left_pressed_and_moved_slot(QMouseEvent*);
    void right_pressed_and_moved_slot(QMouseEvent*);
    void left_and_right_pressed_and_moved_slot(QMouseEvent*);

    void is_solver_running_slot(bool);
signals:
    void print_debug_signal(const QString&);
    void smiley_surprised_queued_signal();
};

#endif // MOUSEINPUT_H
