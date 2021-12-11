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
    QVector<QVector<char>> fieldArray;
    QVector<QVector<char>> minesArray;
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
    QVector<QVector<char>> createArray();
    void createCell();
    void fillMinesArray(const Common::Coords&);
    void printNumber(const Common::Coords&, const int&);
    Common::Coords gridPosition(Cell*);
    QVector<Common::Coords> findNeighbours(const QVector<QVector<char>>& tempArray, const Common::Coords& coords, const char& content);
    void autoReveal(const Common::Coords&, QVector<int>&);
    void flagAutoUncover(const Common::Coords&);
    void gameOver(const Common::Coords&, const QString&);
public:
    explicit Field(QWidget *parent = nullptr, const int& cols = 9, const int& rows = 9, const int& mines = 10, const int& cellSize = 25);
    ~Field();

    // public variables:
    int cellSize;
    int cols;
    int rows;
    int mines;

    // public methods:
    void addCells();
    bool isFlagSet(const Common::Coords&);
    bool isNumber(const Common::Coords&);
private slots:
    void onLeftReleased();
    void onRightReleased();
    void onDoubleClicked();
};

#endif // FIELD_H
