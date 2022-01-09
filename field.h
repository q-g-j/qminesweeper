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
    QVector<QVector<Button*>> buttons2DVector;
    QGridLayout *layout = nullptr;
    Stylesheet stylesheet;
    Button::ButtonIconsStruct buttonIcons;
public:
    explicit Field(QWidget *parent,
                   const Button::ButtonIconsStruct&,
                   const quint16& cols,
                   const quint16& rows,
                   const quint16& mines,
                   const quint16& buttonSize);
    ~Field();

    // public variables:
    QVector<QVector<char>> field2DVector;
    QVector<QVector<char>> mines2DVector;
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
    void print_debug_signal(const QString&);
public slots:
    void solver_place_flag_slot(const Common::Coords&);
    void is_solver_running_slot(bool);
};

#endif // FIELD_H
