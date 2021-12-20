#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QGridLayout>

#include "cell.h"
#include "common.h"

class Field : public QWidget
{
     Q_OBJECT
private:
    // private variables:
    int mines;
    int flagsCount;
    int minesLeft;
    int countUnrevealed;
    bool firstTurn;
    QVector<QVector<char>> field2DVector;
    QVector<QVector<char>> mines2DVector;
    struct buttonStruct
    {
        Cell* button;
        Common::Coords coords;
    };
    QVector<buttonStruct> buttonStructVector;

    Cell** cell;
    QGridLayout *layout;

    QString stylesheet_button_flag;
    QString stylesheet_button_mine;
    QString stylesheet_button_mine_hit;
    QString stylesheet_button_revealed;
    QString stylesheet_button_unrevealed;
    QString stylesheet_button_1;
    QString stylesheet_button_2;
    QString stylesheet_button_3;
    QString stylesheet_button_4;
    QString stylesheet_button_5;
    QString stylesheet_button_6;
    QString stylesheet_button_7;
    QString stylesheet_button_8;

    // private methods:
    QVector<QVector<char>> create2DVector();
    void createCells();
    void fillMinesVector(const Common::Coords&);
    void printNumber(const Common::Coords&, const int&);
    QVector<Common::Coords> findNeighbours(const QVector<QVector<char>>&, const Common::Coords&, const char&);
    void autoReveal(const Common::Coords&, QVector<int>&);
    void flagAutoReveal(const Common::Coords&);
    bool isFlagSet(const Common::Coords&);
    bool isNumber(const Common::Coords&);
    void gameOver(const Common::Coords&, const QString&);
    Common::Coords getButtonCoords(Cell*);

public:
    explicit Field(QWidget *parent = nullptr,
                   const int& cols = 9,
                   const int& rows = 9,
                   const int& mines = 10,
                   const int& cellSize = 25);
    ~Field();

    // public variables:
    int cellSize;
    int cols;
    int rows;
    bool isGameOver;

    // public methods:
    void addCells();
signals:
    void game_started_signal();
    void game_over_signal(const QString&);
    void minesleft_changed_signal(const int&);
    void smiley_surprised_signal();
private slots:
    void on_left_released();
    void on_right_released();
    void on_double_clicked();
};

#endif // FIELD_H
