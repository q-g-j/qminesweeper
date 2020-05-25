#ifndef WRAPPERWIDGET_H
#define WRAPPERWIDGET_H

#include <QWidget>
#include <QGridLayout>

#include "common.h"

#include "myqpushbutton.h"


class Field : public QWidget
{
     Q_OBJECT

private:
    // private variables:
    int flagsCount;
    int minesLeft;
    int countCovered;

    // private methods:
    char** createArray();
    void fillMinesArray(Common::Coords&);
    Common::Coords gridPosition(MyQPushButton*);
    std::vector<Common::Coords> findNeighbours(char** tempArray, Common::Coords const& coords, char const& content);

public:
    explicit Field(QWidget *parent = nullptr, int const& cols = 9, int const& rows = 9, int const& mines = 10, int const& cellSize = 25);
    ~Field();


    // public variables:
    int cellSize;
    int cols;
    int rows;
    int mines;
    bool firstTurn;
    char** fieldArray;
    char** minesArray;

    QGridLayout *layout;
    MyQPushButton** cell;

    // public methods:
    void addCells();
    bool isFlagSet(Common::Coords&);
    bool isNumber(Common::Coords&);

    // getters:
    int getColumns();
    int getRows();

private slots:
    void onLeftPressed();
    void onRightPressed();
    void onLeftReleased();
    void onRightReleased();

protected:
};

#endif // WRAPPERWIDGET_H
