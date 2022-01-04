#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>

#include "common.h"
#include "difficulty.h"
#include "field.h"
#include "mouseinput.h"
#include "solver.h"
#include "stylesheet.h"
#include "timer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QGridLayout *fieldLayout;
    Field *field = nullptr;
    Timer *timer = nullptr;
    Solver *solver = nullptr;
    MouseInput *mouseInput = nullptr;
    Common common;
    Stylesheet stylesheet;
    Difficulty::DifficultyStruct difficulty;

    quint16 buttonSize = 25;
    quint16 minesLeftFrameWidth;
    quint16 minesLeftFrameHeight;
    quint16 timerFrameHeight;
    quint16 minesLeftNumberWidth;
    quint16 spacerMiddleLeftFixedWidth = 7;

    void clearLayout(QLayout*);
    void newGame(const Difficulty::DifficultyStruct&);
    void newGameFromMenu();
    void newGameFromSmiley();
    void newGameRequested(const char&);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setInfoBarNumber(QWidget*, const quint16&);
private slots:
    void on_actionNew_triggered();
    void on_actionQuit_triggered();
    void on_smiley_released();
    void new_game_slot(const Difficulty::DifficultyStruct&);
    void game_over_slot(bool);
    void start_timer_slot();
    void smiley_surprised_slot();
    void minesleft_changed_slot(const qint16&);
    void solver_stopped_slot(const char&);
    void set_infobar_time_slot(const QString&, const quint16&);
protected:
    bool eventFilter(QObject*, QEvent*);
};
#endif // MAINWINDOW_H
