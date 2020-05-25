#ifndef WRAPPERWIDGET_H
#define WRAPPERWIDGET_H

#include <QWidget>
#include <QGridLayout>

#include "common.h"

#include "cell.h"


class Field : public QWidget
{
     Q_OBJECT

private:
    // private variables:
    int flagsCount;
    int minesLeft;
    int countCovered;
    bool gameover;

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
    Common::Coords gridPosition(Cell*);
    std::vector<Common::Coords> findNeighbours(char** tempArray, Common::Coords const& coords, char const& content);
    void gameOver(Common::Coords const&);

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
    Cell** cell;

    // public methods:
    void addCells();
    bool isFlagSet(Common::Coords&);
    bool isNumber(Common::Coords&);

    // getters:
    int getColumns();
    int getRows();

private slots:
    void onDoubleClicked();
    void onLeftPressed();
    void onRightPressed();
    void onLeftReleased();
    void onRightReleased();

protected:
};

#endif // WRAPPERWIDGET_H
