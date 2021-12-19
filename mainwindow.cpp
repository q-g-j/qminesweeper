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
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    // width and height of a cell in pixels:
    this->cellSize = 25;

    this->fieldLayout = new QGridLayout(ui->fieldWrapper);

    QFontDatabase::addApplicationFont(":/fonts/NotoSans-CondensedMedium.ttf");

    QFile smiley                (":/stylesheet/infobar_smiley.css");
    QFile smiley_won            (":/stylesheet/infobar_smiley_won.css");
    QFile smiley_lost           (":/stylesheet/infobar_smiley_lost.css");
    QFile smiley_surprised      (":/stylesheet/infobar_smiley_surprised.css");
    smiley.open                 (QFile::ReadOnly);
    smiley_won.open             (QFile::ReadOnly);
    smiley_lost.open            (QFile::ReadOnly);
    smiley_surprised.open       (QFile::ReadOnly);
    this->stylesheet_smiley           = QLatin1String(smiley.readAll());
    this->stylesheet_smiley_won       = QLatin1String(smiley_won.readAll());
    this->stylesheet_smiley_lost      = QLatin1String(smiley_lost.readAll());
    this->stylesheet_smiley_surprised = QLatin1String(smiley_surprised.readAll());
    smiley.close();
    smiley_won.close();
    smiley_lost.close();
    smiley_surprised.close();

    // start in easy mode:
    this->difficulty.cols = 9;
    this->difficulty.rows = 9;
    this->difficulty.mines = 10;

    this->newGame(this->difficulty);
    this->timer = new Timer(ui->timerSeconds, ui->timerTenSeconds, ui->timerMinutes, ui->timerTenMinutes);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearLayout(QLayout *layout) {
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

void MainWindow::newGame(const Difficulty::DifficultyStruct& difficulty_)
{
    if (this->field != nullptr) delete this->field;
    this->field = nullptr;
    this->clearLayout(this->fieldLayout);
    this->fieldLayout->setSpacing(0);
    this->fieldLayout->setContentsMargins(0,0,0,0);
    this->field = new Field(ui->fieldWrapper, difficulty_.cols, difficulty_.rows, difficulty_.mines, this->cellSize);
    ui->fieldWrapper->setLayout(this->fieldLayout);
    ui->fieldWrapper->setMinimumSize(field->cols * field->cellSize, field->rows * field->cellSize);
    ui->timerTenMinutes->setText("0");
    ui->timerMinutes->setText("0");
    ui->timerTenSeconds->setText("0");
    ui->timerSeconds->setText("0");
    ui->smiley->setStyleSheet(this->stylesheet_smiley);
    this->field->addCells();
    this->fieldLayout->addWidget(this->field);
    this->centralWidget()->adjustSize();
    this->adjustSize();
    this->setFixedSize(this->size().width(), this->size().height());
    this->minesleft_changed(difficulty_.mines);
    connect(this->field, &Field::game_over, this, &MainWindow::game_over);
    connect(this->field, &Field::minesleft_changed, this, &MainWindow::minesleft_changed);
    connect(this->field, &Field::smiley_surprised, this, &MainWindow::smiley_surprised);
}
// open a dialog (difficulty.ui) to choose difficulty:
void MainWindow::on_actionNew_triggered()
{
    Difficulty difficulty_(this);
    connect(&difficulty_, &Difficulty::buttonClicked, this, &MainWindow::new_game_slot);
    difficulty_.setModal(true);
    difficulty_.exec();
}

void MainWindow::new_game_slot(const Difficulty::DifficultyStruct& difficulty)
{
    this->difficulty.cols = difficulty.cols;
    this->difficulty.rows = difficulty.rows;
    this->difficulty.mines = difficulty.mines;
    this->newGame(difficulty);
    this->timer->timerInstance->stop();
    this->timer->counterFine = 0;
    this->timer->timerInstance->start(10);
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
    this->timer->timerInstance->stop();
    this->timer->counterFine = 0;
    this->timer->timerInstance->start(10);
}

void MainWindow::smiley_surprised()
{
    this->sleep(350);
}

void MainWindow::game_over(const QString& mode)
{
    timer->timerInstance->stop();
    if (mode == "lose")
        ui->smiley->setStyleSheet(this->stylesheet_smiley_lost);
    else if (mode == "win")
        ui->smiley->setStyleSheet(this->stylesheet_smiley_won);
}

void MainWindow::minesleft_changed(const int& minesLeft)
{
    ui->labelMinesLeft->setText(QString::number(minesLeft));
}


void MainWindow::sleep(const int& milliseconds)
{
    if (field->isGameOver != true)
    {
        ui->smiley->setStyleSheet(this->stylesheet_smiley_surprised);
        QEventLoop loop;
        QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
        loop.exec();
        if (field->isGameOver != true)
            ui->smiley->setStyleSheet(this->stylesheet_smiley);
    }
}
