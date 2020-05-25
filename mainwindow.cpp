#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "ui_difficulty.h"

#include <QSizePolicy>
#include <QGridLayout>
#include <QPushButton>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DifficultyStruct difficulty;
    difficulty.cols = 9;
    difficulty.rows = 9;
    difficulty.mines = 10;

//    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    newGame(difficulty);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete field;
    field = nullptr;
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
    field = new Field(centralWidget(), difficulty.cols, difficulty.rows, difficulty.mines, 30);
    setCentralWidget(field);
    field->addCells();
    show();
    centralWidget()->setMinimumSize(field->cols * (field->cellSize) + 20, field->rows * (field->cellSize) + 20);
    adjustSize();
}

void MainWindow::on_actionNew_triggered()
{
    Difficulty difficulty(this);

    connect(&difficulty, SIGNAL(buttonClicked(DifficultyStruct)), this, SLOT(new_game_slot(DifficultyStruct)));
    difficulty.setModal(true);
    difficulty.exec();
}

void MainWindow::new_game_slot(DifficultyStruct const& difficulty)
{
    newGame(difficulty);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
