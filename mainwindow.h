#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>
#include "timer.h"

#include "difficulty.h"
#include "field.h"

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
    int cols, rows;
    int cellSize;    
    Difficulty::DifficultyStruct difficulty;

    void clearLayout(QLayout*);
    void newGame(const Difficulty::DifficultyStruct&);

    QString stylesheet_smiley;
    QString stylesheet_smiley_won;
    QString stylesheet_smiley_lost;
    QString stylesheet_smiley_surprised;

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

protected:
};
#endif // MAINWINDOW_H
