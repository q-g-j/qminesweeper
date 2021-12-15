#include <QSizePolicy>
#include <QSize>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // width and height of a cell in pixels:
    this->cellSize = 25;

    // start in easy mode:
    difficulty.cols = 9;
    difficulty.rows = 9;
    difficulty.mines = 10;

    fieldLayout = new QGridLayout(ui->fieldWrapper);

    newGame(difficulty);
    QFile smiley                (":/stylesheet/infobar_smiley.css");
    QFile smiley_pressed        (":/stylesheet/infobar_smiley_pressed.css");
    QFile smiley_won            (":/stylesheet/infobar_smiley_won.css");
    QFile smiley_lost           (":/stylesheet/infobar_smiley_lost.css");
    smiley.open                 (QFile::ReadOnly);
    smiley_pressed.open         (QFile::ReadOnly);
    smiley_won.open             (QFile::ReadOnly);
    smiley_lost.open            (QFile::ReadOnly);
    stylesheet_smiley =         QLatin1String(smiley.readAll());
    stylesheet_smiley_pressed = QLatin1String(smiley_pressed.readAll());
    stylesheet_smiley_won =     QLatin1String(smiley_won.readAll());
    stylesheet_smiley_lost =    QLatin1String(smiley_lost.readAll());
    smiley.close();
    smiley_pressed.close();
    smiley_won.close();
    smiley_lost.close();
    ui->smiley->setStyleSheet(stylesheet_smiley);
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
    if (field != nullptr) delete field;
    if (timer != nullptr) delete timer;
    clearLayout(fieldLayout);
    ui->fieldWrapper->setLayout(fieldLayout);
    fieldLayout->setSpacing(0);
    fieldLayout->setContentsMargins(0,0,0,0);
    timer = new Timer(ui->timerSeconds, ui->timerTenSeconds, ui->timerMinutes, ui->timerTenMinutes);
    field = new Field(ui->fieldWrapper, difficulty.cols, difficulty.rows, difficulty.mines, this->cellSize, ui->labelMinesLeft, ui->smiley, timer);
    ui->fieldWrapper->setMinimumSize(field->cols * (field->cellSize), field->rows * (field->cellSize));
    field->addCells();
    fieldLayout->addWidget(field);
    centralWidget()->adjustSize();
    this->adjustSize();
    this->setFixedSize(this->size().width(), this->size().height());
    ui->smiley->setStyleSheet(stylesheet_smiley);
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
    this->difficulty.cols = difficulty.cols;
    this->difficulty.rows = difficulty.rows;
    this->difficulty.mines = difficulty.mines;
    newGame(difficulty);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_smiley_pressed()
{
    ui->smiley->setStyleSheet(stylesheet_smiley_pressed);
}

void MainWindow::on_smiley_released()
{
    Difficulty::DifficultyStruct difficulty_;
    difficulty_.cols = this->difficulty.cols;
    difficulty_.rows = this->difficulty.rows;
    difficulty_.mines = this->difficulty.mines;
    this->newGame(difficulty_);
}
