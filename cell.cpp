#include <QPushButton>
#include <QMouseEvent>

#include "cell.h"

Cell::Cell(QWidget *parent) : QPushButton(parent)
{
}

void Cell::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit doubleClicked();
}

void Cell::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        emit rightReleased();
    else if(e->button() == Qt::LeftButton)
        emit leftReleased();
}

void Cell::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        emit rightPressed();
    else if(e->button() == Qt::LeftButton)
        emit leftPressed();
}
