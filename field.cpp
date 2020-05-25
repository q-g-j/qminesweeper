#include <QSizePolicy>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QThread>
#include <vector>

#include "common.h"
#include "field.h"

#include "myqpushbutton.h"


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
    this->minesLeft = mines;
    this->countCovered = cols * rows;
    this->flagsCount = 0;
    this->cell = new MyQPushButton*[this->cols + 1];
    for (int i=0; i < (this->cols + 1); i++)
        this->cell[i] = new MyQPushButton[this->rows + 1];

    layout = new QGridLayout;
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->setAlignment(Qt::AlignCenter);
//    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
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
}

char** Field::createArray()
{
    const int colsNum = this->cols + 1;
    const int rowsNum = this->rows + 1;
    char** tempArray = nullptr;
    tempArray = new char*[colsNum];
    for (int i=0; i < colsNum; i++)
        tempArray[i] = new char[rowsNum];
    for (int i = 0; i < colsNum; i++)
        for (int j = 0; j < rowsNum; j++)
            tempArray[i][j] = ' ';
    return tempArray;
}

// place mines at random positions of this->minesArray[][]:
void Field::fillMinesArray(Common::Coords& userFirstInput)
{
    Common::Coords coords;
    int sizeOfFieldArray = this->cols * this->rows;
    std::vector<int> tempVector;
    for (int i = 1; i <= sizeOfFieldArray; i++)
    {
        if (i != Common::structToInt(userFirstInput, this->cols))
            tempVector.push_back(i);
    }

    std::random_shuffle(tempVector.begin(), tempVector.end());
    for (int i = 0; i < this->mines; i++)
    {
        coords = Common::intToStruct(tempVector.at(i), this->cols);
        this->minesArray[coords.col][coords.row] = 'X';
    }
//    for (int i = 1; i <= this->cols; i++)
//    {
//        for (int j = 1; j <= this->rows; j++)
//        {
//            qDebug() << "minesArray[" + QString::number(i) + "][" + QString::number(j) + "]: " + this->minesArray[i][j];
//        }
//    }
}

void Field::addCells()
{
    QString cellStyle =
        "MyQPushButton {"
        "   border-style: inset;"
        "   border-width: 0px;"
        "   border-radius: 0px;"
        "   border-image: url() 0 0 0 0 stretch stretch;"
        "}";

    for (int i = 1; i <= this->cols; i++)
    {
        for (int j = 1; j <= this->rows; j++)
        {
            this->cell[i][j].setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            this->cell[i][j].setStyleSheet(cellStyle);
            this->cell[i][j].setIcon(QIcon(":/images/button_covered.png"));
            this->cell[i][j].setIconSize(QSize(this->cellSize, this->cellSize));
            this->cell[i][j].setFixedSize(this->cellSize, this->cellSize);
            layout->addWidget(&this->cell[i][j], i - 1, j - 1, 1, 1);
            connect(&this->cell[i][j], SIGNAL(leftPressed()), this, SLOT(onLeftPressed()));
            connect(&this->cell[i][j], SIGNAL(rightPressed()), this, SLOT(onRightPressed()));
            connect(&this->cell[i][j], SIGNAL(leftReleased()), this, SLOT(onLeftReleased()));
            connect(&this->cell[i][j], SIGNAL(rightReleased()), this, SLOT(onRightReleased()));
        }
    }
}

Common::Coords Field::gridPosition(MyQPushButton * button)
{
  Common::Coords coords;
  if (! button->parentWidget()) return coords;
  auto layout = qobject_cast<QGridLayout*>(button->parentWidget()->layout());
  if (! layout) return coords;
  int index = layout->indexOf(button);
  Q_ASSERT(index >= 0);
  int _ = 0;
  layout->getItemPosition(index, &coords.col, &coords.row, &_, &_);
  coords.col++;
  coords.row++;
  return coords;
}

// test coords if they contain a flag:
bool Field::isFlagSet(Common::Coords& coords)
{
    if (this->fieldArray[coords.col][coords.row] == 'F')
        return true;
    else
        return false;
}

// test coords if they contain a number:
bool Field::isNumber(Common::Coords& coords)
{
    for (int i = 1; i < 8; i++)
    {
        if (this->fieldArray[coords.col][coords.row] == i + 58)
            return true;
    }
    return false;
}

// find neighbors of a cell at "coords" that hold a given content (passed by variable symbol)
std::vector<Common::Coords> Field::findNeighbours(char** tempArray, Common::Coords const& coords, char const& content)
{
    std::vector<Common::Coords> neighboursVector;

    // up left:
    if (coords.col-1 > 0 && coords.row-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row-1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col-1;
            coordsTemp.row = coords.row-1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // up middle:
    if (coords.row-1 > 0)
    {
        if (tempArray[coords.col][coords.row-1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col;
            coordsTemp.row = coords.row-1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // up right:
    if (coords.col+1 <= this->cols && coords.row-1 > 0)
    {
        if (tempArray[coords.col+1][coords.row-1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row-1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // middle left:
    if (coords.col-1 > 0)
    {
        if (tempArray[coords.col-1][coords.row] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col-1;
            coordsTemp.row = coords.row;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // middle right:
    if (coords.col+1 <= this->cols)
    {
        if (tempArray[coords.col+1][coords.row] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // down left:
    if (coords.col-1 > 0 && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col-1][coords.row+1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col-1;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // down middle:
    if (coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col][coords.row+1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    // down right:
    if (coords.col+1 <= this->cols && coords.row+1 <= this->rows)
    {
        if (tempArray[coords.col+1][coords.row+1] == content)
        {
            Common::Coords coordsTemp;
            coordsTemp.col = coords.col+1;
            coordsTemp.row = coords.row+1;
            neighboursVector.push_back(coordsTemp);
        }
    }

    return neighboursVector;
}

void Field::onLeftPressed()
{
    MyQPushButton *button = qobject_cast<MyQPushButton *>(sender());
    button->setDown(true);
}

void Field::onRightPressed()
{
    MyQPushButton *button = qobject_cast<MyQPushButton *>(sender());
    button->setDown(true);
}

void Field::onLeftReleased()
{
    MyQPushButton *button = qobject_cast<MyQPushButton *>(sender());
    auto coordsTemp = gridPosition(button);
    std::vector<Common::Coords> neighboursMinesVector;

    // fill minesArray with mines ONLY once after users first guess:
    if (this->firstTurn)
        fillMinesArray(coordsTemp);

    // if user hit a mine, reveal the game field - game is lost:
    if (this->minesArray[coordsTemp.col][coordsTemp.row] == 'X')
    {
        this->minesArray[coordsTemp.col][coordsTemp.row] = 'H';
        for (int i = 1; i <= this->cols; i++)
        {
            for (int j = 1; j <= this->rows; j++)
            {
                if (this->minesArray[i][j] == 'X')
                {
                    this->cell[i][j].setIcon(QIcon(":/images/button_mine.png"));
                }
                else if (this->minesArray[i][j] == 'H')
                {
                    this->cell[i][j].setIcon(QIcon(":/images/button_mine_hit.png"));
                }
                else if (this->minesArray[i][j] == ' ')
                {
                    this->cell[i][j].setIcon(QIcon(":/images/button_uncovered.png"));
                }
            }
        }
    }

    else
    {
        // uncover the players choice and place the number of surrounding mines in it:
        neighboursMinesVector = findNeighbours(this->minesArray, coordsTemp, 'X');
        if (neighboursMinesVector.size() == 0)
        {
            this->fieldArray[coordsTemp.col][coordsTemp.row] = 'U';
            this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_uncovered.png"));
        }
        else
        {
            if (neighboursMinesVector.size() == 1)
            {
                this->fieldArray[coordsTemp.col][coordsTemp.row] = '1';
                this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_1.png"));
            }
            if (neighboursMinesVector.size() == 2)
            {
                this->fieldArray[coordsTemp.col][coordsTemp.row] = '2';
                this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_2.png"));
            }
            if (neighboursMinesVector.size() == 3)
            {
                this->fieldArray[coordsTemp.col][coordsTemp.row] = '3';
                this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_3.png"));
            }
            if (neighboursMinesVector.size() == 4)
            {
                this->fieldArray[coordsTemp.col][coordsTemp.row] = '4';
                this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_4.png"));
            }
            if (neighboursMinesVector.size() == 5)
            {
                this->fieldArray[coordsTemp.col][coordsTemp.row] = '5';
                this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_5.png"));
            }
            if (neighboursMinesVector.size() == 6)
            {
                this->fieldArray[coordsTemp.col][coordsTemp.row] = '6';
                this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_6.png"));
            }
            if (neighboursMinesVector.size() == 7)
            {
                this->fieldArray[coordsTemp.col][coordsTemp.row] = '7';
                this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_7.png"));
            }
            if (neighboursMinesVector.size() == 8)
            {
                this->fieldArray[coordsTemp.col][coordsTemp.row] = '8';
                this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_8.png"));
            }
        }
        this->countCovered--;
    }

    // automatically uncover all neighbour squares of squares containing a 'U' (repeat if new 'U's appears):
    bool run = true;
    if (neighboursMinesVector.size() == 0)
    {
        while (run == true)
        {
            // for each free position do:
            for (int i = 1; i <= this->cols; i++)
            {
                for (int j = 1; j <= this->rows; j++)
                {
                    if (this->fieldArray[i][j] == ' ')
                    {
                        // create a new vector of neighbours containing a 'U':
                        Common::Coords coordsBase;
                        coordsBase.col = i;
                        coordsBase.row = j;
                        std::vector<Common::Coords> neighboursZerosVector;
                        neighboursZerosVector = findNeighbours(this->fieldArray, coordsBase, 'U');


                        // if there is a neighbour containing a 'U' create a new vector of neighbours containing mines:
                        if (neighboursZerosVector.size() != 0)
                        {
                            std::vector<Common::Coords> neighboursMinesVectorNew;
                            neighboursMinesVectorNew = findNeighbours(this->minesArray, coordsBase, 'X');

                            // place neighboursMinesVectorNew.size() in fieldArray:
                            Common::Coords coordsTemp;
                            coordsTemp.col = i;
                            coordsTemp.row = j;
                            if (neighboursMinesVectorNew.size() == 0)
                            {
                                this->fieldArray[i][j] = 'U';
                                this->cell[i][j].setIcon(QIcon(":/images/button_uncovered.png"));
                            }
                            else
                            {
                                if (neighboursMinesVectorNew.size() == 1)
                                {
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = '1';
                                    this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_1.png"));
                                }
                                if (neighboursMinesVectorNew.size() == 2)
                                {
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = '2';
                                    this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_2.png"));
                                }
                                if (neighboursMinesVectorNew.size() == 3)
                                {
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = '3';
                                    this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_3.png"));
                                }
                                if (neighboursMinesVectorNew.size() == 4)
                                {
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = '4';
                                    this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_4.png"));
                                }
                                if (neighboursMinesVectorNew.size() == 5)
                                {
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = '5';
                                    this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_5.png"));
                                }
                                if (neighboursMinesVectorNew.size() == 6)
                                {
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = '6';
                                    this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_6.png"));
                                }
                                if (neighboursMinesVectorNew.size() == 7)
                                {
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = '7';
                                    this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_7.png"));
                                }
                                if (neighboursMinesVectorNew.size() == 8)
                                {
                                    this->fieldArray[coordsTemp.col][coordsTemp.row] = '8';
                                    this->cell[coordsTemp.col][coordsTemp.row].setIcon(QIcon(":/images/button_8.png"));
                                }
                            }
                            this->countCovered--;
                        }
                    }
                }
            }
            run = false;

            // repeat if necessary:
            for (int a = 1; a <= this->cols; a++)
            {
                for (int b = 1; b <= this->rows; b++)
                {
                    if (this->fieldArray[a][b] == ' ')
                    {
                        // create a new vector of neighbours containing 'U':
                        Common::Coords coordsBaseNew;
                        coordsBaseNew.col = a;
                        coordsBaseNew.row = b;
                        std::vector<Common::Coords> neighboursZerosVectorNew;
                        neighboursZerosVectorNew = findNeighbours(this->fieldArray, coordsBaseNew, 'U');
                        if (neighboursZerosVectorNew.size() != 0)
                            run = true;
                    }
                }
            }
        }
    }


    this->firstTurn = false;
}

void Field::onRightReleased()
{
    MyQPushButton *button = qobject_cast<MyQPushButton *>(sender());    
    auto coords = gridPosition(button);
    if (this->fieldArray[coords.col][coords.row] == ' ' || this->fieldArray[coords.col][coords.row] == 'F')
    {
        if (! isFlagSet(coords))
        {
            button->setIcon(QIcon(":/images/button_flag.png"));
            this->fieldArray[coords.col][coords.row] = 'F';
            this->flagsCount++;
            this->minesLeft--;
            this->countCovered--;
        }
        else
        {
            button->setIcon(QIcon(":/images/button_covered.png"));
            this->fieldArray[coords.col][coords.row] = ' ';
            this->flagsCount--;
            this->minesLeft++;
            this->countCovered++;
        }
    }
}
