#include <QSizePolicy>
#include <QSize>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fieldlayout.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    Difficulty::DifficultyStruct difficulty;
    fieldWrapperLayout = new FieldLayout;
    ui->fieldwrapper->setLayout(fieldWrapperLayout);
    difficulty.cols = 9;
    difficulty.rows = 9;
    difficulty.mines = 10;
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

void MainWindow::newGame(Difficulty::DifficultyStruct const& difficulty)
{
    field = new Field(difficulty.cols, difficulty.rows, difficulty.mines, 30);
    field->addCells();
    clearLayout(fieldWrapperLayout);
    fieldWrapperLayout->addWidget(field);
    connect(this, SIGNAL(colsChanged(const int&)), fieldWrapperLayout, SLOT(onColsChanged(const int&)));
    connect(this, SIGNAL(rowsChanged(const int&)), fieldWrapperLayout, SLOT(onRowsChanged(const int&)));
    emit colsChanged(field->cols);
    emit rowsChanged(field->rows);
    centralWidget()->adjustSize();
    ui->fieldwrapper->setMinimumSize(field->cols * (field->cellSize), field->rows * (field->cellSize));
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
