#include <QDebug>
#include <QSizePolicy>
#include <QFontDatabase>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "timer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

    labelMinesLeftFrameWidth = ui->labelMinesLeftFrame->width();
    labelMinesLeftFrameHeight = ui->labelMinesLeftFrame->height();
    timerFrameHeight = ui->timerFrame->height();
    minesLeftNumberWidth = ui->labelMinesLeftOnes->width();
    spacerMiddleLeftFixedWidth = 8;

    this->fieldLayout = new QGridLayout(ui->fieldWrapper);

    QFontDatabase::addApplicationFont(":/fonts/digital-7.ttf");

    // width and height of a button in pixels:
    this->buttonSize = 25;

    // start in easy mode:
    this->difficulty.cols = 9;
    this->difficulty.rows = 9;
    this->difficulty.mines = 10;

//    ui->menuCheat->menuAction()->setVisible(false);

    this->newGame(this->difficulty);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// delete all items from a layout:
// (found here: https://stackoverflow.com/a/4857631)
void MainWindow::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0)))
    {
        if (item->layout())
        {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget())
        {
           delete item->widget();
        }
        delete item;
    }
}

void MainWindow::newGame(const Difficulty::DifficultyStruct& difficulty_)
{
    if (this->field != nullptr)
    {
        delete this->field;
        this->field = nullptr;
    }
    if (this->timer != nullptr)
    {
        delete this->timer;
        this->timer = nullptr;
    }

    this->field = new Field(ui->fieldWrapper, difficulty_.cols, difficulty_.rows, difficulty_.mines, this->buttonSize);
    this->clearLayout(this->fieldLayout);
    this->fieldLayout->setSpacing(0);
    this->fieldLayout->setContentsMargins(0,0,0,0);
    this->fieldLayout->addWidget(this->field);
    ui->fieldWrapper->setLayout(this->fieldLayout);
    ui->fieldWrapper->setMinimumSize(field->cols * field->buttonSize, field->rows * field->buttonSize);

    connect(this->field, &Field::game_over_signal, this, &MainWindow::game_over_slot);
    connect(this->field, &Field::minesleft_changed_signal, this, &MainWindow::minesleft_changed_slot);
    connect(this->field, &Field::smiley_surprised_signal, this, &MainWindow::smiley_surprised_slot);
    connect(this->field, &Field::game_started_signal, this, &MainWindow::start_timer_slot);
    connect(&this->solver, &Solver::place_remove_flag_signal, field, &Field::place_remove_flags_slot);
//    connect(this->field, &Field::field_debug_signal, this, &MainWindow::field_debug_slot);

    this->minesleft_changed_slot(difficulty_.mines);

    ui->timerTenMinutes->setText("0");
    ui->timerMinutes->setText("0");
    ui->timerTenSeconds->setText("0");
    ui->timerSeconds->setText("0");
    ui->smiley->setStyleSheet(this->stylesheet.stylesheet_smiley);

    if (difficulty_.mines < 100)
    {
        ui->labelMinesLeftTens->show();
        ui->labelMinesLeftOnes->show();
        ui->labelMinesLeftThousands->hide();
        ui->labelMinesLeftHundreds->hide();
        ui->labelMinesLeftFrame->resize(labelMinesLeftFrameWidth - 2 * minesLeftNumberWidth, labelMinesLeftFrameHeight);
        ui->spacerMiddleLeftFixed->changeSize(spacerMiddleLeftFixedWidth + 2 * minesLeftNumberWidth, labelMinesLeftFrameHeight);
    }
    else if (difficulty_.mines < 1000)
    {
        ui->labelMinesLeftHundreds->show();
        ui->labelMinesLeftTens->show();
        ui->labelMinesLeftOnes->show();
        ui->labelMinesLeftThousands->hide();
        ui->labelMinesLeftFrame->resize(labelMinesLeftFrameWidth - minesLeftNumberWidth, labelMinesLeftFrameHeight);
        ui->spacerMiddleLeftFixed->changeSize(spacerMiddleLeftFixedWidth + minesLeftNumberWidth, labelMinesLeftFrameHeight);
    }
    else
    {
        ui->labelMinesLeftThousands->show();
        ui->labelMinesLeftHundreds->show();
        ui->labelMinesLeftTens->show();
        ui->labelMinesLeftOnes->show();
        ui->labelMinesLeftFrame->resize(labelMinesLeftFrameWidth, labelMinesLeftFrameHeight);
        ui->spacerMiddleLeftFixed->changeSize(spacerMiddleLeftFixedWidth, labelMinesLeftFrameHeight);
    }

    // after changing the size of a spacer, need to invalidate its parent's layout:
    ui->infoBarLayout->invalidate();

    ui->labelMinesLeftFrame->adjustSize();
    ui->timerFrame->adjustSize();
    this->centralWidget()->adjustSize();
    this->adjustSize();
    this->setFixedSize(this->size().width(), this->size().height());

    this->timer = new Timer(ui->timerSeconds, ui->timerTenSeconds, ui->timerMinutes, ui->timerTenMinutes);
}

void MainWindow::field_debug_slot()
{
    qDebug() << QString::number(this->timer->counterFine);
}

// open a dialog (difficulty.ui) to choose difficulty:
void MainWindow::on_actionNew_triggered()
{
    if (field->isSolverRunning != true)
    {
        Difficulty difficulty_(this);
        connect(&difficulty_, &Difficulty::button_clicked_signal, this, &MainWindow::new_game_slot);
        difficulty_.setModal(true);
        difficulty_.exec();
    }
}

void MainWindow::new_game_slot(const Difficulty::DifficultyStruct& difficulty)
{
    this->difficulty.cols = difficulty.cols;
    this->difficulty.rows = difficulty.rows;
    this->difficulty.mines = difficulty.mines;
    this->newGame(difficulty);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_smiley_released()
{
    if (field->isSolverRunning != true)
    {
        Difficulty::DifficultyStruct difficulty_;
        difficulty_.cols = this->difficulty.cols;
        difficulty_.rows = this->difficulty.rows;
        difficulty_.mines = this->difficulty.mines;
        this->newGame(difficulty_);
    }
}

void MainWindow::smiley_surprised_slot()
{
    if (field->isGameOver != true)
    {
        ui->smiley->setStyleSheet(this->stylesheet.stylesheet_smiley_surprised);
        Common::sleep(350);
        if (field->isGameOver != true)
        {
            ui->smiley->setStyleSheet(this->stylesheet.stylesheet_smiley);
        }
    }
}

void MainWindow::game_over_slot(bool hasLost)
{
    this->timer->timerStop();
    if (hasLost == true)
    {
        ui->smiley->setStyleSheet(this->stylesheet.stylesheet_smiley_lost);
    }
    else if (hasLost == false)
    {
        ui->smiley->setStyleSheet(this->stylesheet.stylesheet_smiley_won);
    }
}

void MainWindow::minesleft_changed_slot(const int& minesLeft)
{
    if (minesLeft < 0)
    {
        ui->labelMinesLeftOnes->setText("0");
    }
    else if (minesLeft < 10)
    {
        ui->labelMinesLeftOnes->setText(QString::number(minesLeft));
        ui->labelMinesLeftTens->setText("");
        ui->labelMinesLeftHundreds->setText("");
        ui->labelMinesLeftThousands->setText("");
    }
    else if (minesLeft < 100)
    {
        int ones = minesLeft % 10;
        int tens = minesLeft / 10;
        ui->labelMinesLeftOnes->setText(QString::number(ones));
        ui->labelMinesLeftTens->setText(QString::number(tens));
        ui->labelMinesLeftHundreds->setText("");
        ui->labelMinesLeftThousands->setText("");
    }
    else if (minesLeft < 1000)
    {
        int ones = minesLeft % 10;
        int tens = (minesLeft % 100) / 10;
        int hundreds = minesLeft / 100;
        ui->labelMinesLeftOnes->setText(QString::number(ones));
        ui->labelMinesLeftTens->setText(QString::number(tens));
        ui->labelMinesLeftHundreds->setText(QString::number(hundreds));
        ui->labelMinesLeftThousands->setText("");
    }
    else
    {
        int ones = minesLeft % 10;
        int tens = ((minesLeft % 1000) % 100) / 10;
        int hundreds = (minesLeft % 1000) / 100;
        int thousands = minesLeft / 1000;
        ui->labelMinesLeftOnes->setText(QString::number(ones));
        ui->labelMinesLeftTens->setText(QString::number(tens));
        ui->labelMinesLeftHundreds->setText(QString::number(hundreds));
        ui->labelMinesLeftThousands->setText(QString::number(thousands));
    }
}

void MainWindow::start_timer_slot()
{
    this->timer->timerStart();
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (field->isSolverRunning != true)
    {
        if (e->key() == Qt::Key_F)
        {
            solver.autoSolve(*field, true, false, false);
        }
        else if (e->key() == Qt::Key_R)
        {
            solver.autoSolve(*field, false, true, false);

            // check if player has won:
            if (field->flagsCount + field->countUnrevealed == field->mines)
            {
                Common::Coords dummyCoords;
                field->gameOver(dummyCoords, false);
            }
        }
        else if (e->key() == Qt::Key_S)
        {
            solver.autoSolve(*field, true, true, true);

            // check if player has won:
            if (field->flagsCount + field->countUnrevealed == field->mines)
            {
                Common::Coords dummyCoords;
                field->gameOver(dummyCoords, false);
            }
        }
    }
}
