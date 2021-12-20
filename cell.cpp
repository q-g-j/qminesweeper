#include <QMouseEvent>
#include <QPushButton>

#include "cell.h"

Cell::Cell(QWidget *parent) : QPushButton(parent)
{
}

void Cell::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit this->double_clicked_signal();
}

void Cell::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        emit this->right_released_signal();
    else if(e->button() == Qt::LeftButton)
        emit this->left_released_signal();
}
