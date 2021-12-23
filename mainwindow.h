#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>
#include "timer.h"

#include "difficulty.h"
#include "field.h"
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
    Stylesheet stylesheet;

    int cols, rows;
    int cellSize;
    int labelMinesLeftFrameWidth;
    int labelMinesLeftFrameHeight;
    int timerFrameHeight;
    int minesLeftNumberWidth;
    int spacerMiddleLeftFixedWidth;
    Difficulty::DifficultyStruct difficulty;

    void clearLayout(QLayout*);
    void newGame(const Difficulty::DifficultyStruct&);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_actionNew_triggered();
    void on_actionQuit_triggered();
    void on_smiley_released();
    void new_game_slot(const Difficulty::DifficultyStruct&);
    void smiley_surprised_slot();
    void game_over_slot(const QString&);
    void minesleft_changed_slot(const int&);
    void start_timer_slot();
    void on_actionPlace_Flags_triggered();

protected:
};
#endif // MAINWINDOW_H
