#ifndef WRAPPERWIDGET_H
#define WRAPPERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

#include "common.h"


class Field : public QWidget
{
     Q_OBJECT
private:
    int** createArray();
    void fillMinesArray(Common::Coords&);
    Common::Coords gridPosition(QWidget*);

public:
    explicit Field(QWidget *parent = nullptr, int const& cols = 9, int const& rows = 9, int const& mines = 10, int const& cellSize = 25);
    ~Field();

    QGridLayout *layout;
    int cellSize;
    int cols;
    int rows;
    int mines;
    bool firstTurn;
    int** fieldArray;
    int** minesArray;
    QPushButton** cell;

    // methods:
    void addCells();

    // getters:
    int getColumns();
    int getRows();
private slots:
    void onCellLeftClicked();

protected:
};

#endif // WRAPPERWIDGET_H
