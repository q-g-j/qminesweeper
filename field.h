#ifndef FIELD_H
#define FIELD_H

#include <QSize>
#include <QWidget>
#include <QGridLayout>

#include "cell.h"
#include "common.h"

class Field : public QWidget
{
     Q_OBJECT
private:
    // private variables:
    int flagsCount;
    int minesLeft;
    int countCovered;
    bool gameover;
    bool firstTurn;
    char** fieldArray;
    char** minesArray;
    Cell** cell;

    QGridLayout *layout;

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
    void printNumber(Common::Coords const&, int const&);
    Common::Coords gridPosition(Cell*);
    std::vector<Common::Coords> findNeighbours(char** tempArray, Common::Coords const& coords, char const& content);
    void autoReveal(std::vector<Common::Coords>&);
    void gameOver(Common::Coords const&, QString const&);
public:
    explicit Field(QWidget *parent = nullptr, int const& cols = 9, int const& rows = 9, int const& mines = 10, int const& cellSize = 25);
    ~Field();

    // public variables:
    int cellSize;
    int cols;
    int rows;
    int mines;

    // public methods:
    void addCells();
    bool isFlagSet(Common::Coords&);
    bool isNumber(Common::Coords&);
private slots:
    void onLeftReleased();
    void onRightReleased();
    void onDoubleClicked();
};

#endif // FIELD_H
