#include <QApplication>
#include <QMouseEvent>
#include <QPushButton>
#include <QDebug>

#include "button.h"

Button::Button(QWidget *parent) : QPushButton(parent)
{
    installEventFilter(this);
}

bool Button::eventFilter(QObject* object, QEvent *e)
{
    if (object == this && e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

        if (mouseEvent->buttons() == (Qt::LeftButton | Qt::RightButton))
        {
            this->leftandrightbuttonpressedflag = true;
            emit this->left_and_right_pressed_signal();
        }
        else if (mouseEvent->button() == Qt::LeftButton)
        {
            this->leftbuttonpressedflag = true;
            emit this->left_pressed_signal();
        }
        else if (mouseEvent->button() == Qt::RightButton)
        {
            this->rightbuttonpressedflag = true;
            emit this->right_pressed_signal();
        }
    }

    if (object == this && e->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

        if (this->leftandrightbuttonpressedflag
                && mouseEvent->buttons() != (Qt::LeftButton | Qt::RightButton))
        {
            emit this->left_and_right_released_signal();
        }
        else if (mouseEvent->button() == (Qt::LeftButton))
        {
            emit this->left_released_signal();
        }
        else if(mouseEvent->button() == (Qt::RightButton))
        {
            emit this->right_released_signal();
        }
        this->leftbuttonpressedflag = false;
        this->rightbuttonpressedflag = false;
        this->leftandrightbuttonpressedflag = false;
    }

    if (object == this && e->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

        if (this->leftbuttonpressedflag)
        {
            if (! this->leftandrightbuttonpressedflag)
            {
                emit this->left_pressed_and_moved_signal(mouseEvent);
            }
        }
        if (this->leftandrightbuttonpressedflag)
        {
            emit this->left_and_right_pressed_and_moved_signal(mouseEvent);
        }
    }
    return false;
}
