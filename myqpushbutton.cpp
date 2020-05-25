#include <QPushButton>
#include <QMouseEvent>

#include "myqpushbutton.h"

MyQPushButton::MyQPushButton(QWidget *parent) : QPushButton(parent)
{
}

void MyQPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        emit rightReleased();
    else if(e->button() == Qt::LeftButton)
        emit leftReleased();
}

void MyQPushButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        emit rightPressed();
    else if(e->button() == Qt::LeftButton)
        emit leftPressed();
}
