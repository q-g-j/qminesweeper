#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QGridLayout>

#include "cell.h"
#include "common.h"
#include "icons.h"

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
    bool tempRevealed;
    QPoint mouseCurrentPosition;
    struct buttonStruct
    {
        Cell* button;
        Common::Coords coords;
    };
    QVector<buttonStruct> buttonStructVector;

    Cell** cell;
    QGridLayout *layout;

    Icons icons;

    // private methods:
    QVector<QVector<char>> create2DVector();
    void createCells();
    void fillMinesVector(const Common::Coords&);
    void printNumber(const Common::Coords&, const int&);
    void autoReveal(const Common::Coords&, QVector<int>&);
    bool isFlagSet(const Common::Coords&);
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
    QVector<QVector<char>> field2DVector;
    QVector<QVector<char>> mines2DVector;
    int cellSize;
    int cols;
    int rows;
    bool isGameOver;

    // public methods:
    void addCells();
    bool isNumber(const Common::Coords&);
    QVector<Common::Coords> findNeighbours(const QVector<QVector<char>>&, const Common::Coords&, const char&);
    void flagAutoReveal(const Common::Coords&);
signals:
    void game_started_signal();
    void game_over_signal(const QString&);
    void minesleft_changed_signal(const int&);
    void smiley_surprised_signal();
    void left_pressed_signal();
private slots:
    void on_left_pressed();
    void on_left_released();
    void on_right_released();
    void on_double_clicked();
    void on_left_pressed_and_moved(QMouseEvent*);
};

#endif // FIELD_H
