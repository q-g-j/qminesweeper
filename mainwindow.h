#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>

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
    Field *field;
    int cols, rows;
    int cellSize;

    void newGame(const Difficulty::DifficultyStruct&);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void new_game_slot(const Difficulty::DifficultyStruct&);
    void on_actionNew_triggered();
    void on_actionQuit_triggered();
protected:
};
#endif // MAINWINDOW_H
