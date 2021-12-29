#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>
#include "timer.h"

#include "difficulty.h"
#include "field.h"
#include "solver.h"
#include "stylesheet.h"

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
    Stylesheet stylesheet;
    Difficulty::DifficultyStruct difficulty;

    int buttonSize;
    int minesLeftFrameWidth;
    int minesLeftFrameHeight;
    int timerFrameHeight;
    int minesLeftNumberWidth;
    int spacerMiddleLeftFixedWidth;

    void clearLayout(QLayout*);
    void newGame(const Difficulty::DifficultyStruct&);
    void newGameFromMenu();
    void newGameFromSmiley();
    void newGameRequested(const char&);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setInfoBarNumber(QWidget*, const int&);
private slots:
    void on_actionNew_triggered();
    void on_actionQuit_triggered();
    void on_smiley_released();
    void new_game_slot(const Difficulty::DifficultyStruct&);
    void smiley_surprised_slot();
    void game_over_slot(bool);
    void minesleft_changed_slot(const int&);
    void start_timer_slot();
    void field_debug_slot();
    void solver_stopped_slot(const char&);
    void set_infobar_time_slot(const QString&, const int&);
protected:
    void keyReleaseEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
