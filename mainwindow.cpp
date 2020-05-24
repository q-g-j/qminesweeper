#include "mainwindow.h"
#include "field.h"
#include "keepsquareandcentered.h"

#include "ui_mainwindow.h"
#include "ui_difficulty.h"

#include <QSizePolicy>
#include <QGridLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DifficultyStruct difficulty;
    difficulty.cols = 9;
    difficulty.rows = 9;
    difficulty.mines = 10;

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    newGame(difficulty);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void clearLayout(QLayout * layout) {
   if (! layout)
      return;
   while (auto item = layout->takeAt(0)) {
      delete item->widget();
      clearLayout(item->layout());
   }
}

void MainWindow::newGame(DifficultyStruct const& difficulty)
{
    hide();
    Field *field = new Field(this, difficulty.cols, difficulty.rows, difficulty.mines, 25);
    field->addCells();
    setCentralWidget(field);
    adjustSize();
    show();
}

void MainWindow::on_actionNew_triggered()
{
    Difficulty *difficulty = new Difficulty;
    connect(difficulty, SIGNAL(buttonClicked(DifficultyStruct)), this, SLOT(new_game_slot(DifficultyStruct)));
    difficulty->show();
}

void MainWindow::new_game_slot(DifficultyStruct const& difficulty)
{
    newGame(difficulty);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
