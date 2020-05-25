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

    QString stylesheet_button_flag;
    QString stylesheet_button_mine;
    QString stylesheet_button_mine_hit;
    QString stylesheet_button_covered;
    QString stylesheet_button_uncovered;
    QString stylesheet_button_1;
    QString stylesheet_button_2;
    QString stylesheet_button_3;
    QString stylesheet_button_4;
    QString stylesheet_button_5;
    QString stylesheet_button_6;
    QString stylesheet_button_7;
    QString stylesheet_button_8;

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
