#include <QRect>
#include <QGridLayout>
#include <QDebug>

#include "fieldlayout.h"

FieldLayout::FieldLayout(QWidget *parent) :
    QGridLayout(parent)
{
    setSpacing(0);
    setMargin(0);
}

void FieldLayout::setGeometry(const QRect& oldRect)
{
    QRect newRect;
    float ratio = static_cast<float>(this->cols) / static_cast<float>(this->rows);
    if (this->cols == this->rows)
    {
            int min = std::min(oldRect.width(), oldRect.height());
            newRect = QRect(0, 0, min, min);
    }
    else if (this->cols > this->rows)
    {
        if (oldWidth == oldRect.width())
        {
            newRect = QRect(0, 0, oldRect.width(), oldRect.width()/ratio);
        }
        else if (oldHeight == oldRect.height())
        {
            newRect = QRect(0, 0, oldRect.height()*ratio, oldRect.height());
        }
        else
        {
            newRect = QRect(0, 0, oldRect.width(), oldRect.width()/ratio);
            newRect = QRect(0, 0, oldRect.height()*ratio, oldRect.height());
        }
    }
    newRect.moveCenter(oldRect.center());
    itemAt(0)->setGeometry(newRect);
    this->oldWidth = oldRect.width();
    this->oldHeight = oldRect.height();
}

void FieldLayout::onColsChanged(int const& cols)
{
    this->cols = cols;
}

void FieldLayout::onRowsChanged(int const& rows)
{
    this->rows = rows;
}
