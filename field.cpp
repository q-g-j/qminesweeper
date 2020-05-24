#include <QPushButton>
#include <QSizePolicy>
#include <QDebug>
#include <QThread>
#include <vector>

#include "common.h"
#include "field.h"


Field::Field(QWidget *parent, int const& cols, int const& rows, int const& mines, int const& cellSize)
: QWidget(parent)
{
    this->firstTurn = true;
    this->cols = cols;
    this->rows = rows;
    this->mines = mines;
    this->cellSize = cellSize;
    this->fieldArray = createArray();
    this->minesArray = createArray();
    this->cell = new QPushButton*[this->cols + 1];
    for (int i=0; i < (this->cols + 1); i++)
        this->cell[i] = new QPushButton[this->rows + 1];

    layout = new QGridLayout;
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
    setFixedSize(this->cols * (this->cellSize + 1), this->rows * (this->cellSize + 1));
}

Field::~Field()
{
    // delete pointer (2D-)arrays to avoid memory leaks:
    for (int i=0; i <= this->cols; i++)
    {
        delete[] this->fieldArray[i];
    }
    delete[] this->fieldArray;

    for (int i=0; i <= this->cols; i++)
    {
        delete[] this->minesArray[i];
    }
    delete[] this->minesArray;

    for (int i=0; i <= this->cols; i++)
    {
        delete[] this->cell[i];
    }
    delete[] this->cell;
    qDebug() << "Deconstructor of Field working";
}

int** Field::createArray()
{
    const int colsNum = this->cols + 1;
    const int rowsNum = this->rows + 1;
    int** tempArray = nullptr;
    tempArray = new int*[colsNum];
    for (int i=0; i < colsNum; i++)
        tempArray[i] = new int[rowsNum];
    for (int i = 0; i < colsNum; i++)
        for (int j = 0; j < rowsNum; j++)
            tempArray[i][j] = 0;
    return tempArray;
}

// place mines at random positions of this->minesArray[][]:
void Field::fillMinesArray(Common::Coords& userFirstInput)
{
    Common common;
    Common::Coords coords;
    int sizeOfFieldArray = this->cols * this->rows;
    std::vector<int> tempVector;
    for (int i = 1; i <= sizeOfFieldArray; i++)
    {
        if (i != common.structToInt(userFirstInput, this->cols))
            tempVector.push_back(i);
    }

    std::random_shuffle(tempVector.begin(), tempVector.end());
    for (int i = 0; i < this->mines; i++)
    {
        coords = common.intToStruct(tempVector.at(i), this->cols);
        this->minesArray[coords.col][coords.row] = 'X';
    }
}

void Field::addCells()
{
    QString cellStyle =
        "QPushButton {"
        "   border-style: inset;"
        "   border-width: 1px;"
        "   border-radius: 5px;"
        "   background-color: #F0FFFF;"
        "   font-size: 70%;"
        "}";

    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            this->cell[i][j].setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            this->cell[i][j].setStyleSheet(cellStyle);
            this->cell[i][j].setFixedSize(this->cellSize, this->cellSize);
            layout->addWidget(&this->cell[i][j], i - 1, j - 1, 1, 1);
            connect(&this->cell[i][j], SIGNAL(released()), this, SLOT(onCellLeftClicked()));
//            QThread::msleep(100);
        }
    }
}

Common::Coords Field::gridPosition(QWidget * widget) {
  Common::Coords pos;
  if (! widget->parentWidget()) return pos;
  auto layout = qobject_cast<QGridLayout*>(widget->parentWidget()->layout());
  if (! layout) return pos;
  int index = layout->indexOf(widget);
  Q_ASSERT(index >= 0);
  int _;
  layout->getItemPosition(index, &pos.row, &pos.col, &_, &_);
  pos.col += 1;
  pos.row += 1;
  return pos;
}

void Field::onCellLeftClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    auto coords = gridPosition(button);
    if (this->firstTurn)
        fillMinesArray(coords);
    if (this->minesArray[coords.col][coords.row] == 'X')
        button->setText("X");
    else
        button->setText("-");
    this->firstTurn = false;
}
