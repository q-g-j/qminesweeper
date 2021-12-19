#include <QDebug>
#include <QSizePolicy>
#include <QFontDatabase>
#include <QThread>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    // width and height of a cell in pixels:
    this->cellSize = 25;

    fieldLayout = new QGridLayout(ui->fieldWrapper);

    QFontDatabase::addApplicationFont(":/fonts/NotoSans-CondensedMedium.ttf");

    QFile smiley                (":/stylesheet/infobar_smiley.css");
    QFile smiley_won            (":/stylesheet/infobar_smiley_won.css");
    QFile smiley_lost           (":/stylesheet/infobar_smiley_lost.css");
    QFile smiley_surprised      (":/stylesheet/infobar_smiley_surprised.css");
    smiley.open                 (QFile::ReadOnly);
    smiley_won.open             (QFile::ReadOnly);
    smiley_lost.open            (QFile::ReadOnly);
    smiley_surprised.open       (QFile::ReadOnly);
    stylesheet_smiley           = QLatin1String(smiley.readAll());
    stylesheet_smiley_won       = QLatin1String(smiley_won.readAll());
    stylesheet_smiley_lost      = QLatin1String(smiley_lost.readAll());
    stylesheet_smiley_surprised = QLatin1String(smiley_surprised.readAll());
    smiley.close();
    smiley_won.close();
    smiley_lost.close();
    smiley_surprised.close();

    // start in easy mode:
    difficulty.cols = 9;
    difficulty.rows = 9;
    difficulty.mines = 10;

    newGame(difficulty);
    timer = new Timer(ui->timerSeconds, ui->timerTenSeconds, ui->timerMinutes, ui->timerTenMinutes);
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
    field = nullptr;
    clearLayout(fieldLayout);
    fieldLayout->setSpacing(0);
    fieldLayout->setContentsMargins(0,0,0,0);
    field = new Field(ui->fieldWrapper, difficulty.cols, difficulty.rows, difficulty.mines, this->cellSize);
    ui->fieldWrapper->setLayout(fieldLayout);
    ui->fieldWrapper->setMinimumSize(field->cols * field->cellSize, field->rows * field->cellSize);
    ui->smiley->setStyleSheet(stylesheet_smiley);
    field->addCells();
    fieldLayout->addWidget(field);
    centralWidget()->adjustSize();
    this->adjustSize();
    this->setFixedSize(this->size().width(), this->size().height());
    minesleft_changed(difficulty.mines);
    connect(field, &Field::game_over, this, &MainWindow::game_over);
    connect(field, &Field::minesleft_changed, this, &MainWindow::minesleft_changed);
    connect(field, &Field::smiley_surprised, this, &MainWindow::smiley_surprised);
}
// open a dialog (difficulty.ui) to choose difficulty:
void MainWindow::on_actionNew_triggered()
{
    Difficulty difficulty(this);
    connect(&difficulty, &Difficulty::buttonClicked, this, &MainWindow::new_game_slot);
    difficulty.setModal(true);
    difficulty.exec();
}

void MainWindow::new_game_slot(const Difficulty::DifficultyStruct& difficulty)
{
    this->difficulty.cols = difficulty.cols;
    this->difficulty.rows = difficulty.rows;
    this->difficulty.mines = difficulty.mines;
    newGame(difficulty);
    timer->timerInstance->stop();
    timer->counterFine = 0;
    timer->timerInstance->start(10);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_smiley_released()
{
    Difficulty::DifficultyStruct difficulty_;
    difficulty_.cols = this->difficulty.cols;
    difficulty_.rows = this->difficulty.rows;
    difficulty_.mines = this->difficulty.mines;
    this->newGame(difficulty_);
    timer->counterFine = 0;
    timer->timerInstance->stop();
    timer->timerInstance->start(10);
}

void MainWindow::smiley_surprised()
{
    sleep(500);
}

void MainWindow::game_over(const QString& mode)
{
    timer->timerInstance->stop();
    timer->counterFine = 0;
    if (mode == "lose")
        ui->smiley->setStyleSheet(stylesheet_smiley_lost);
    else if (mode == "win")
        ui->smiley->setStyleSheet(stylesheet_smiley_won);
}

void MainWindow::minesleft_changed(const int& minesLeft)
{
    ui->labelMinesLeft->setText(QString::number(minesLeft));
}


void MainWindow::sleep(const int& milliseconds)
{
    ui->smiley->setStyleSheet(stylesheet_smiley_surprised);
    QEventLoop loop;

    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);

    loop.exec();
    ui->smiley->setStyleSheet(stylesheet_smiley);
}
