#include <QMouseEvent>
#include <QPushButton>

#include "cell.h"

Cell::Cell(QWidget *parent) : QPushButton(parent)
{
}

void Cell::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit this->doubleClicked();
}

void Cell::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        emit this->rightReleased();
    else if(e->button() == Qt::LeftButton)
        emit this->leftReleased();
}
