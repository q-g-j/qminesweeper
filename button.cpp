#include <QApplication>
#include <QMouseEvent>
#include <QDebug>

#include "button.h"

Button::Button(QWidget *parent) : QLabel(parent)
{
    this->installEventFilter(this);
}

Button::~Button() {}

bool Button::eventFilter(QObject* object, QEvent *e)
{
    if (object == this && e->type() == QEvent::MouseButtonPress)
    {
        MouseStruct mouseEventStruct;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

        if (mouseEvent->buttons() == (Qt::LeftButton | Qt::RightButton))
        {
//            emit this->print_debug_signal("left and right pressed");
            this->leftandrightbuttonpressedflag = true;
            mouseEventStruct.reason = MouseEnum::leftAndRightPressed;
            mouseEventStruct.mouseEvent = mouseEvent;
            emit this->mouse_event_signal(mouseEventStruct);
        }
        else if (
                 mouseEvent->button() == Qt::LeftButton
                 && this->leftandrightbuttonpressedflag == false
                 )
        {
//            emit this->print_debug_signal("left pressed");
            mouseEventStruct.reason = MouseEnum::leftPressed;
            mouseEventStruct.mouseEvent = mouseEvent;
            emit this->mouse_event_signal(mouseEventStruct);
        }
        else if (
                 mouseEvent->button() == Qt::RightButton
                 && this->leftandrightbuttonpressedflag == false
                 )
        {
//            emit this->print_debug_signal("right pressed");
            mouseEventStruct.reason = MouseEnum::rightPressed;
            mouseEventStruct.mouseEvent = mouseEvent;
            emit this->mouse_event_signal(mouseEventStruct);
        }
    }

    else if (object == this && e->type() == QEvent::MouseButtonRelease)
    {
        MouseStruct mouseEventStruct;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

        if (
                this->leftandrightbuttonpressedflag == true
                && mouseEvent->button() == (Qt::LeftButton)
                && mouseEvent->buttons() != (Qt::RightButton)
                )
        {
//            emit this->print_debug_signal("left and right released from left");
            this->leftandrightbuttonpressedflag = false;
            mouseEventStruct.reason = MouseEnum::leftAndRightReleased;
            emit this->mouse_event_signal(mouseEventStruct);
        }
        else if (
                 this->leftandrightbuttonpressedflag == true
                 && mouseEvent->button() == (Qt::RightButton)
                 && mouseEvent->buttons() != (Qt::LeftButton)
                 )
        {
//            emit this->print_debug_signal("left and right released from right");
            this->leftandrightbuttonpressedflag = false;
            mouseEventStruct.reason = MouseEnum::leftAndRightReleased;
            emit this->mouse_event_signal(mouseEventStruct);
        }
        else if (
                 this->leftandrightbuttonpressedflag == true
                 && mouseEvent->button() == (Qt::LeftButton)
                 && mouseEvent->buttons() == (Qt::RightButton)
                 )
        {
//            emit this->print_debug_signal("right pressed and left released");
            mouseEventStruct.reason = MouseEnum::leftAndRightReleased;
            emit this->mouse_event_signal(mouseEventStruct);
        }
        else if (
                 this->leftandrightbuttonpressedflag == true
                 && mouseEvent->button() == (Qt::RightButton)
                 && mouseEvent->buttons() == (Qt::LeftButton)
                 )
        {
//            emit this->print_debug_signal("left pressed and right released");
            mouseEventStruct.reason = MouseEnum::leftAndRightReleased;
            emit this->mouse_event_signal(mouseEventStruct);
        }
        else if (
                 mouseEvent->button() == Qt::LeftButton
                 && mouseEvent->buttons() != (Qt::RightButton)
                 )
        {
//            emit this->print_debug_signal("left released");
            mouseEventStruct.reason = MouseEnum::leftReleased;
            emit this->mouse_event_signal(mouseEventStruct);
        }
        else if (
                 mouseEvent->button() == Qt::RightButton
                 && mouseEvent->buttons() != (Qt::LeftButton)
                 )
        {
//            emit this->print_debug_signal("right released");
            mouseEventStruct.reason = MouseEnum::rightReleased;
            emit this->mouse_event_signal(mouseEventStruct);
        }
    }

    else if (object == this && e->type() == QEvent::MouseMove)
    {
        MouseStruct mouseEventStruct;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

        if (mouseEvent->buttons() == (Qt::LeftButton | Qt::RightButton))
        {
//            emit this->print_debug_signal("left and right pressed and moved");
            mouseEventStruct.mouseEvent = mouseEvent;
            mouseEventStruct.reason = MouseEnum::leftAndRightPressedAndMoved;
            emit this->mouse_event_signal(mouseEventStruct);
        }
        else if (
                 mouseEvent->buttons() == Qt::LeftButton
                 && this->leftandrightbuttonpressedflag == false
                 )
        {
//            emit this->print_debug_signal("left pressed and moved");
            mouseEventStruct.mouseEvent = mouseEvent;
            mouseEventStruct.reason = MouseEnum::leftPressedAndMoved;
            emit this->mouse_event_signal(mouseEventStruct);
        }
        else if (
                 mouseEvent->buttons() == Qt::RightButton
                 && this->leftandrightbuttonpressedflag == false
                 )
        {
//            emit this->print_debug_signal("right pressed and moved");
            mouseEventStruct.mouseEvent = mouseEvent;
            mouseEventStruct.reason = MouseEnum::rightPressedAndMoved;
            emit this->mouse_event_signal(mouseEventStruct);
        }
    }
    return false;
}
