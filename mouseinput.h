#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include <QMouseEvent>
#include <QObject>

#include "button.h"
#include "common.h"
#include "debug.h"
#include "field.h"
#include "stylesheet.h"

class MouseInput : public QObject
{
    Q_OBJECT
private:
    Field *field;
    Stylesheet stylesheet;
    Button::ButtonIconsStruct buttonIcons;

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
    MouseInput(Field*, const Button::ButtonIconsStruct&);
    ~MouseInput();

    bool isGameOver = false;
    bool isNewGameRequested = false;
public slots:
    void connect_button_slot(Button*);
    void mouse_event_slot(const Button::MouseStruct&);
    void is_solver_running_slot(bool);
signals:
    void print_debug_signal(const QString&);
    void smiley_surprised_signal();
};

#endif // MOUSEINPUT_H
