#include <QApplication>
#include <QMouseEvent>
#include <QPushButton>
#include <QDebug>

#include "cell.h"

Cell::Cell(QWidget *parent) : QPushButton(parent)
{
    installEventFilter(this);
}

void Cell::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit this->double_clicked_signal();
    }
}

void Cell::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit this->left_released_signal();
    }
    else if(e->button() == Qt::RightButton)
    {
        emit this->right_released_signal();
    }
}

void Cell::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit this->left_pressed_signal();
    }
}

bool Cell::eventFilter(QObject* /*object*/, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress && QApplication::mouseButtons() == Qt::LeftButton)
    {
        this->leftbuttonpressedflag = true;
    }

    if (e->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
        if (this->leftbuttonpressedflag)
        {
            emit this->left_pressed_and_moved_signal(mouseEvent);
        }
    }

    return false;
}
