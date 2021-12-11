#include <QRect>
#include <QGridLayout>
#include <QDebug>

#include "fieldlayout.h"

FieldLayout::FieldLayout(QWidget *parent, const int& cellSize) :
    QGridLayout(parent)
{
    setSpacing(0);
    setContentsMargins(0,0,0,0);

    this->cellSize = cellSize;
}

void FieldLayout::setGeometry(const QRect& oldRect)
{
    if (oldRect.width() == this->cols * 30 && oldRect.height() == this->rows * this->cellSize)
        newRect = QRect(0, 0, oldRect.width(), oldRect.height());

    float ratio = static_cast<float>(this->cols) / static_cast<float>(this->rows);

    // if field is a square:
    if (this->cols == this->rows)
    {
        int min = std::min(oldRect.width(), oldRect.height());
        newRect = QRect(0, 0, min, min);
    }

    // if one side is longer than the other:
    else
    {
        newRect = QRect(0, 0, oldRect.height()*ratio, oldRect.height());
        while (newRect.width() > oldRect.width() || newRect.height() > oldRect.height())
            newRect = QRect(0, 0, (newRect.height() - 1)*ratio, (newRect.height() - 1));
    }

    // keep field in the center:
    newRect.moveCenter(oldRect.center());

    // set field to new Geometry (newRect):
    itemAt(0)->setGeometry(newRect);
}

void FieldLayout::onColsChanged(const int& cols)
{
    this->cols = cols;
    this->colsChanged = true;
}

void FieldLayout::onRowsChanged(const int& rows)
{
    this->rows = rows;
}
