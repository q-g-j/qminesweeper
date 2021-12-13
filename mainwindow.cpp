#include <QSizePolicy>
#include <QSize>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // width and height of a cell in pixels:
    this->cellSize = 25;

    Difficulty::DifficultyStruct difficulty;

    // start in easy mode:
    difficulty.cols = 9;
    difficulty.rows = 9;
    difficulty.mines = 10;

    fieldLayout = new QGridLayout(ui->fieldWrapper);

    newGame(difficulty);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
           delete item->widget();
        }
        delete item;
    }
}

void MainWindow::newGame(const Difficulty::DifficultyStruct& difficulty)
{
    clearLayout(fieldLayout);
    ui->fieldWrapper->setLayout(fieldLayout);
    fieldLayout->setSpacing(0);
    fieldLayout->setContentsMargins(0,0,0,0);

    field = new Field(ui->fieldWrapper, difficulty.cols, difficulty.rows, difficulty.mines, this->cellSize, ui->labelMinesLeft);
    ui->fieldWrapper->setMinimumSize(field->cols * (field->cellSize), field->rows * (field->cellSize));
    field->addCells();
    fieldLayout->addWidget(field);
    centralWidget()->adjustSize();
    this->adjustSize();
    this->setFixedSize(this->size().width(), this->size().height());
}

// open a dialog (difficulty.ui) to choose difficulty:
void MainWindow::on_actionNew_triggered()
{
    Difficulty difficulty(this);
    connect(&difficulty, SIGNAL(buttonClicked(Difficulty::DifficultyStruct)), this, SLOT(new_game_slot(Difficulty::DifficultyStruct)));
    difficulty.setModal(true);
    difficulty.exec();
}

void MainWindow::new_game_slot(const Difficulty::DifficultyStruct& difficulty)
{
    newGame(difficulty);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
