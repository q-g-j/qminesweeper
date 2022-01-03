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
    Common *common;
    Field *field;
    Stylesheet *stylesheet;

    bool isLeftAndRightPressed;

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
signals:
    void print_debug_signal(const QString&);
};

#endif // MOUSEINPUT_H
