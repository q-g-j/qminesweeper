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
    QGridLayout *layout;

    // private methods:
    void create2DVectors();
    void fillMines2DVector(const Common::Coords&);
    void setNumber(const Common::Coords&, const int&);
    void printNumber(const Common::Coords&, const int&);
    void autoReveal(const Common::Coords&, QVector<int>&, bool);
    bool isFlagSet(const Common::Coords&);
    Common::Coords getCoordsFromButton(Button*);
    Common::Coords getCoordsFromMousePosition();

public:
    explicit Field(QWidget *parent = nullptr,
                   const int& cols = 9,
                   const int& rows = 9,
                   const int& mines = 10,
                   const int& buttonSize = 25);
    ~Field();

    // public variables:
    QVector<QVector<char>> field2DVector;
    QVector<buttonStruct> buttonStructVector;
    int buttonSize;
    int cols;
    int rows;
    int mines;
    int flagsCount;
    int minesLeft;
    int countUnrevealed;
    bool isGameOver;
    bool isSolverRunning;
    bool hasSolverStopped;
    bool isNewGameRequested;

    // public methods:
    void setButtonIcon(Button*, const QString&);
    Button* getButtonFromCoords(const Common::Coords&);
    bool isNumber(const Common::Coords&);
    QVector<Common::Coords> findNeighbours(const QVector<QVector<char>>&, const Common::Coords&, const char&);
    void flagAutoReveal(const Common::Coords&, bool, bool);
    void gameOver(const Common::Coords&, bool);
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
public slots:
    void place_remove_flags_slot(const Common::Coords&);
};

#endif // FIELD_H
