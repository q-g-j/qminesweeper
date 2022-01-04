#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QGridLayout>
#include <QPixmap>

#include "button.h"
#include "common.h"
#include "stylesheet.h"

class Field : public QWidget
{
     Q_OBJECT
private:
    // private variables:
    struct buttonStruct
    {
        Button* button = nullptr;
        Common::Coords coords;
    };
    QVector<QVector<Button*>> buttons2DVector;
    QGridLayout *layout = nullptr;
    Stylesheet stylesheet;
public:
    explicit Field(QWidget *parent = nullptr,
                   const quint16& cols = 9,
                   const quint16& rows = 9,
                   const quint16& mines = 10,
                   const quint16& buttonSize = 25);
    ~Field();

    // public variables:
    QPixmap button_1;
    QPixmap button_2;
    QPixmap button_3;
    QPixmap button_4;
    QPixmap button_5;
    QPixmap button_6;
    QPixmap button_7;
    QPixmap button_8;
    QPixmap button_mine;
    QPixmap button_mine_hit;
    QPixmap button_revealed;
    QPixmap button_unrevealed;
    QPixmap button_pressed;
    QPixmap button_flag;

    QVector<QVector<char>> field2DVector;
    QVector<QVector<char>> mines2DVector;
    QVector<buttonStruct> buttonStructVector;
    quint16 buttonSize;
    quint16 cols;
    quint16 rows;
    quint16 mines;
    qint16 minesLeft;
    quint16 countUnrevealed;
    quint16 flagsCount = 0;
    bool firstTurn = true;
    bool isGameOver = false;
    bool isSolverRunning = false;
    bool isNewGameRequested = false;

    // public methods:
    void create2DVectors();
    bool isNumber(const Common::Coords&);
    void setNumber(const Common::Coords&, const quint16&);
    void printNumber(const Common::Coords&, const quint16&);
    void fillMines2DVector(const Common::Coords&);
    Common::Coords getCoordsFromButton(Button*);
    Button* getButtonFromCoords(const Common::Coords&);
    QVector<Common::Coords> findNeighbours(const QVector<QVector<char>>&, const Common::Coords&, const char&);
    void autoReveal(const Common::Coords&, QVector<quint16>&, bool);
    void flagAutoReveal(const Common::Coords&, bool, bool);
    void gameOver(const Common::Coords&, bool);
    void setButtonIcon(Button*, const QPixmap&);
signals:
    void connect_button_signal(Button*);
    void game_started_signal();
    void game_over_signal(bool);
    void minesleft_changed_signal(const quint16&);
    void smiley_surprised_signal();
public slots:
    void solver_place_flag_slot(const Common::Coords&);
    void is_solver_running_slot(bool);
};

#endif // FIELD_H
