#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
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
    QGridLayout *layout;
    Field *field;

    void newGame(DifficultyStruct const&);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int cols, rows;

private slots:
    void new_game_slot(DifficultyStruct const&);

//protected:
//    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void on_actionNew_triggered();
    void on_actionQuit_triggered();
};
#endif // MAINWINDOW_H
