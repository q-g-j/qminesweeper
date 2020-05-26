#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Difficulty::DifficultyStruct difficulty;
    difficulty.cols = 9;
    difficulty.rows = 9;
    difficulty.mines = 10;
    newGame(difficulty);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void clearLayout(QLayout * layout) {
//   if (! layout)
//      return;
//   while (auto item = layout->takeAt(0)) {
//      delete item->widget();
//      clearLayout(item->layout());
//   }
//}

void MainWindow::newGame(Difficulty::DifficultyStruct const& difficulty)
{
    hide();
    field = new Field(this, difficulty.cols, difficulty.rows, difficulty.mines, 30);
    setCentralWidget(field);
    field->addCells();
    show();
    centralWidget()->setMinimumSize(field->cols * (field->cellSize) + 20, field->rows * (field->cellSize) + 20);
    adjustSize();
}

// open a dialog (difficulty.ui) to choose difficulty:
void MainWindow::on_actionNew_triggered()
{
    Difficulty difficulty(this);
    connect(&difficulty, SIGNAL(buttonClicked(Difficulty::DifficultyStruct)), this, SLOT(new_game_slot(Difficulty::DifficultyStruct)));
    difficulty.setModal(true);
    difficulty.exec();
}

void MainWindow::new_game_slot(Difficulty::DifficultyStruct const& difficulty)
{
    newGame(difficulty);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
