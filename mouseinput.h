#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include <QMouseEvent>
#include <QObject>

#include "button.h"
#include "common.h"
#include "field.h"

class MouseInput : public QObject
{
    Q_OBJECT
private:
    Field *field;
    bool isLeftAndRightPressed;

    Common::Coords lastButtonCoords;
    Common::Coords pressedButtonCoords;
    Common::Coords leftAndRightPressedButtonCoords;
    QPoint currentMousePosition;
    QVector<Common::Coords> leftAndRightPressedNeighboursCoveredVector;

    void leftPressed();
    void rightPressed();
    void leftReleased();
    void rightReleased(const Common::Coords&);
    void leftAndRightPressed(const Common::Coords&);
    void leftAndRightReleased(const Common::Coords&);
    void leftPressedAndMoved(QMouseEvent*);
    Common::Coords getCoordsFromMousePosition();
public:
    MouseInput(Field*);
    ~MouseInput();
public slots:
    void left_pressed_slot();
    void right_pressed_slot();
    void left_released_slot();
    void right_released_slot();
    void left_and_right_pressed_slot();
    void left_and_right_released_slot();
    void left_pressed_and_moved_slot(QMouseEvent*);
    void left_and_right_pressed_and_moved_slot(QMouseEvent*);
    void connect_button_slot(Button*);
signals:
    void print_debug_signal(const QString&);
};

#endif // MOUSEINPUT_H
