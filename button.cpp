#include <QApplication>
#include <QMouseEvent>
#include <QPushButton>
#include <QDebug>

#include "button.h"

Button::Button(QWidget *parent) : QPushButton(parent)
{
    this->installEventFilter(this);
}

Button::~Button()
{
}

bool Button::eventFilter(QObject* object, QEvent *e)
{
    if (object == this && e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

        if (mouseEvent->buttons() == (Qt::LeftButton | Qt::RightButton))
        {
//            emit this->print_debug_signal("left and right pressed");
            this->leftandrightbuttonpressedflag = true;
            emit this->left_and_right_pressed_signal(mouseEvent);
        }
        else if (mouseEvent->button() == Qt::LeftButton
                 && ! this->leftandrightbuttonpressedflag)
        {
//            emit this->print_debug_signal("left pressed");
            emit this->left_pressed_signal(mouseEvent);
        }
        else if (mouseEvent->button() == Qt::RightButton
                 && ! this->leftandrightbuttonpressedflag)
        {
//            emit this->print_debug_signal("right pressed");
            emit this->right_pressed_signal(mouseEvent);
        }
    }

    else if (object == this && e->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

        if (this->leftandrightbuttonpressedflag
                && mouseEvent->button() == (Qt::LeftButton)
                && mouseEvent->buttons() != (Qt::RightButton))
        {
//            emit this->print_debug_signal("left and right released from left");
            this->leftandrightbuttonpressedflag = false;
            emit this->left_and_right_released_signal();
        }
        else if (this->leftandrightbuttonpressedflag
                && mouseEvent->button() == (Qt::RightButton)
                && mouseEvent->buttons() != (Qt::LeftButton))
        {
//            emit this->print_debug_signal("left and right released from right");
            this->leftandrightbuttonpressedflag = false;
            emit this->left_and_right_released_signal();
        }
        else if (this->leftandrightbuttonpressedflag
                && mouseEvent->button() == (Qt::LeftButton)
                && mouseEvent->buttons() == (Qt::RightButton))
        {
//            emit this->print_debug_signal("right pressed and left released");
            emit this->left_and_right_released_signal();
        }
        else if (this->leftandrightbuttonpressedflag
                && mouseEvent->button() == (Qt::RightButton)
                && mouseEvent->buttons() == (Qt::LeftButton))
        {
//            emit this->print_debug_signal("left pressed and right released");
            emit this->left_and_right_released_signal();
        }
        else if (mouseEvent->button() == Qt::LeftButton
                 && mouseEvent->buttons() != (Qt::RightButton))
        {
//            emit this->print_debug_signal("left released");
            emit this->left_released_signal();
        }
        else if (mouseEvent->button() == Qt::RightButton
                 && mouseEvent->buttons() != (Qt::LeftButton))
        {
//            emit this->print_debug_signal("right released");
            emit this->right_released_signal();
        }
    }

    else if (object == this && e->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

        if (mouseEvent->buttons() == (Qt::LeftButton | Qt::RightButton))
        {
//            emit this->print_debug_signal("left and right pressed and moved");
            emit this->left_and_right_pressed_and_moved_signal(mouseEvent);
        }
        else if (mouseEvent->buttons() == Qt::LeftButton
                 && ! this->leftandrightbuttonpressedflag)
        {
//            emit this->print_debug_signal("left pressed and moved");
            emit this->left_pressed_and_moved_signal(mouseEvent);
        }
        else if (mouseEvent->buttons() == Qt::RightButton
                 && ! this->leftandrightbuttonpressedflag)
        {
//            emit this->print_debug_signal("right pressed and moved");
            emit this->right_pressed_and_moved_signal(mouseEvent);
        }
    }
    return false;
}
