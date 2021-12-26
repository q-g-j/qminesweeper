#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QGridLayout>

#include "button.h"
#include "common.h"
#include "stylesheet.h"

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
    QPoint currentMousePosition;
    Common::Coords leftPressedButtonCoords;
    Common::Coords lastButtonCoords;
    struct buttonStruct
    {
        Button* button;
        Common::Coords coords;
    };
    QVector<QVector<char>> mines2DVector;
    QVector<QVector<Button*>> buttons2DVector;
    QVector<buttonStruct> buttonsVector;
    QGridLayout *layout;
    Stylesheet *stylesheet;

    // private methods:
    void setButtonIcon(Button*, const QString&);
    void create2DVectors();
    void fillMines2DVector(const Common::Coords&);
    void printNumber(const Common::Coords&, const int&);
    void autoReveal(const Common::Coords&, QVector<int>&);
    bool isFlagSet(const Common::Coords&);
    void gameOver(const Common::Coords&, bool);
    Common::Coords getCoordsFromButton(Button*);
    Button* getButtonFromCoords(const Common::Coords&);
    Common::Coords getCoordsFromMousePosition();

public:
    explicit Field(QWidget *parent = nullptr,
                   Stylesheet *stylesheet = nullptr,
                   const int& cols = 9,
                   const int& rows = 9,
                   const int& mines = 10,
                   const int& buttonSize = 25);
    ~Field();

    // public variables:
    QVector<QVector<char>> field2DVector;
    int buttonSize;
    int cols;
    int rows;
    bool isGameOver;

    // public methods:
    bool isNumber(const Common::Coords&);
    QVector<Common::Coords> findNeighbours(const QVector<QVector<char>>&, const Common::Coords&, const char&);
    void flagAutoReveal(const Common::Coords&);
signals:
    void game_started_signal();
    void game_over_signal(bool);
    void minesleft_changed_signal(const int&);
    void smiley_surprised_signal();
    void left_pressed_signal();
    void field_debug_signal();
private slots:
    void on_left_pressed();
    void on_left_released();
    void on_right_released();
    void on_double_clicked();
    void on_left_pressed_and_moved(QMouseEvent*);
};

#endif // FIELD_H
