#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>

#include "difficulty.h"
#include "field.h"
#include "fieldlayout.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    FieldLayout *fieldWrapperLayout;
    Field *field;
    int cols, rows;

    void newGame(Difficulty::DifficultyStruct const&);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void new_game_slot(Difficulty::DifficultyStruct const&);
    void on_actionNew_triggered();
    void on_actionQuit_triggered();

protected:
signals:
    void colsChanged(int const&);
    void rowsChanged(int const&);
};
#endif // MAINWINDOW_H
