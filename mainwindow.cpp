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

    this->minesLeftFrameWidth = ui->minesLeftFrame->width();
    this->minesLeftFrameHeight = ui->minesLeftFrame->height();
    this->timerFrameHeight = ui->timerFrame->height();
    this->minesLeftNumberWidth = ui->minesLeftOnes->width();
    this->spacerMiddleLeftFixedWidth = 8;

    this->fieldLayout = new QGridLayout(ui->fieldWrapper);

    /*
    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/CursedTimerUlil-Aznm.ttf");

    // get the font names from the font filenames:
    for (int i = 0; i < fontDatabase.families(QFontDatabase::Any).size(); i++)
    {
        QStringList fontlist = fontDatabase.applicationFontFamilies(i);
        for (int j = 0; j < fontlist.size(); j++)
        {
            qDebug() << fontlist.at(j);
        }
    }
    */

    // width and height of a button in pixels:
    this->buttonSize = 25;

    // start in easy mode:
    this->difficulty.cols = 9;
    this->difficulty.rows = 9;
    this->difficulty.mines = 10;

    this->newGame(this->difficulty);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    if (this->solver != nullptr)
    {
        delete this->solver;
        this->solver = nullptr;
    }
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
    if (this->solver != nullptr)
    {
        delete this->solver;
        this->solver = nullptr;
    }

    this->timer = new Timer();
    solver = new Solver;
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
    connect(this->solver, &Solver::solver_stopped_signal, this, &MainWindow::solver_stopped_slot);
    connect(this->solver, &Solver::place_flag_signal, field, &Field::place_flag_slot);
    connect(timer, &Timer::set_infobar_time_signal, this, &MainWindow::set_infobar_time_slot);
//    connect(this->field, &Field::field_debug_signal, this, &MainWindow::field_debug_slot);

    this->minesleft_changed_slot(difficulty_.mines);

    this->setInfoBarNumber(ui->timerTenMinutes, 0);
    this->setInfoBarNumber(ui->timerMinutes, 0);
    this->setInfoBarNumber(ui->timerTenSeconds, 0);
    this->setInfoBarNumber(ui->timerSeconds, 0);
    ui->smiley->setStyleSheet(this->stylesheet.stylesheet_smiley);

    if (difficulty_.mines < 100)
    {
        ui->minesLeftTens->show();
        ui->minesLeftOnes->show();
        ui->minesLeftThousands->hide();
        ui->minesLeftHundreds->hide();
        ui->minesLeftFrame->resize(minesLeftFrameWidth - 2 * minesLeftNumberWidth, minesLeftFrameHeight);
        ui->spacerMiddleLeftFixed->changeSize(spacerMiddleLeftFixedWidth + 2 * minesLeftNumberWidth, minesLeftFrameHeight);
    }
    else if (difficulty_.mines < 1000)
    {
        ui->minesLeftHundreds->show();
        ui->minesLeftTens->show();
        ui->minesLeftOnes->show();
        ui->minesLeftThousands->hide();
        ui->minesLeftFrame->resize(minesLeftFrameWidth - minesLeftNumberWidth, minesLeftFrameHeight);
        ui->spacerMiddleLeftFixed->changeSize(spacerMiddleLeftFixedWidth + minesLeftNumberWidth, minesLeftFrameHeight);
    }
    else
    {
        ui->minesLeftThousands->show();
        ui->minesLeftHundreds->show();
        ui->minesLeftTens->show();
        ui->minesLeftOnes->show();
        ui->minesLeftFrame->resize(minesLeftFrameWidth, minesLeftFrameHeight);
        ui->spacerMiddleLeftFixed->changeSize(spacerMiddleLeftFixedWidth, minesLeftFrameHeight);
    }

    // after changing the size of a spacer, need to invalidate its parent's layout:
    ui->infoBarLayout->invalidate();

    ui->minesLeftFrame->adjustSize();
    ui->timerFrame->adjustSize();
    this->centralWidget()->adjustSize();
    this->adjustSize();
    this->setFixedSize(this->size().width(), this->size().height());
}

void MainWindow::newGameRequested(const char& from)
{
    if (from == 'm')
    {
        solver->isNewGameRequestedFromMenu = true;
        field->isNewGameRequested = true;
    }
    else if (from == 's')
    {
        solver->isNewGameRequestedFromSmiley = true;
        field->isNewGameRequested = true;
    }
}

void MainWindow::newGameFromMenu()
{
    Difficulty difficulty_(this);
    connect(&difficulty_, &Difficulty::button_clicked_signal, this, &MainWindow::new_game_slot);
    difficulty_.setModal(false);
    difficulty_.exec();
}

void MainWindow::newGameFromSmiley()
{
    Difficulty::DifficultyStruct difficulty_;
    difficulty_.cols = this->difficulty.cols;
    difficulty_.rows = this->difficulty.rows;
    difficulty_.mines = this->difficulty.mines;
    this->newGame(difficulty_);
}

void MainWindow::setInfoBarNumber(QWidget *widget, const int &number)
{
    if (number == 0)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_0);
    }
    else if (number == 1)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_1);
    }
    else if (number == 2)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_2);
    }
    else if (number == 3)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_3);
    }
    else if (number == 4)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_4);
    }
    else if (number == 5)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_5);
    }
    else if (number == 6)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_6);
    }
    else if (number == 7)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_7);
    }
    else if (number == 8)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_8);
    }
    else if (number == 9)
    {
        widget->setStyleSheet(stylesheet.stylesheet_digital_9);
    }
}

void MainWindow::field_debug_slot()
{
    qDebug() << QString::number(this->timer->counterFine);
}

// open a dialog (difficulty.ui) to choose difficulty:
void MainWindow::on_actionNew_triggered()
{
    if (solver->isSolverRunning)
    {
        this->newGameRequested('m');
    }
    else
    {
        this->newGameFromMenu();
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
    if (solver->isSolverRunning)
    {
        this->newGameRequested('s');
    }
    else
    {
        this->newGameFromSmiley();
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
        ui->minesLeftOnes->setStyleSheet(stylesheet.stylesheet_digital_0);
    }
    else if (minesLeft < 10)
    {
        this->setInfoBarNumber(ui->minesLeftOnes, minesLeft);
        ui->minesLeftTens->setStyleSheet("QWidget { border-image: none; }");
        ui->minesLeftHundreds->setStyleSheet("QWidget { border-image: none; }");
        ui->minesLeftThousands->setStyleSheet("QWidget { border-image: none; }");
    }
    else if (minesLeft < 100)
    {
        int ones = minesLeft % 10;
        int tens = minesLeft / 10;
        this->setInfoBarNumber(ui->minesLeftOnes, ones);
        this->setInfoBarNumber(ui->minesLeftTens, tens);
        ui->minesLeftHundreds->setStyleSheet("QWidget { border-image: none; }");
        ui->minesLeftThousands->setStyleSheet("QWidget { border-image: none; }");
    }
    else if (minesLeft < 1000)
    {
        int ones = minesLeft % 10;
        int tens = (minesLeft % 100) / 10;
        int hundreds = minesLeft / 100;
        this->setInfoBarNumber(ui->minesLeftOnes, ones);
        this->setInfoBarNumber(ui->minesLeftTens, tens);
        this->setInfoBarNumber(ui->minesLeftHundreds, hundreds);
        ui->minesLeftThousands->setStyleSheet("QWidget { border-image: none; }");
    }
    else
    {
        int ones = minesLeft % 10;
        int tens = ((minesLeft % 1000) % 100) / 10;
        int hundreds = (minesLeft % 1000) / 100;
        int thousands = minesLeft / 1000;
        this->setInfoBarNumber(ui->minesLeftOnes, ones);
        this->setInfoBarNumber(ui->minesLeftTens, tens);
        this->setInfoBarNumber(ui->minesLeftHundreds, hundreds);
        this->setInfoBarNumber(ui->minesLeftThousands, thousands);
    }
}

void MainWindow::start_timer_slot()
{
    this->timer->timerStart();
}

void MainWindow::solver_stopped_slot(const char& from)
{
    if (from == 'm')
    {
        this->newGameFromSmiley();
        this->newGameFromMenu();
    }
    else if (from == 's')
    {
        this->newGameFromSmiley();
    }
}

void MainWindow::set_infobar_time_slot(const QString& t, const int& number)
{
    if (t == "seconds")
    {
        this->setInfoBarNumber(ui->timerSeconds, number);
    }
    else if (t == "tenSeconds")
    {
        this->setInfoBarNumber(ui->timerTenSeconds, number);
    }
    else if (t == "minutes")
    {
        this->setInfoBarNumber(ui->timerMinutes, number);
    }
    else if (t == "tenMinutes")
    {
        this->setInfoBarNumber(ui->timerTenMinutes, number);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (field->isSolverRunning != true && field->isGameOver != true)
    {
        if (e->key() == Qt::Key_F)
        {
            solver->autoSolve(*field, true, false, false);
        }
        else if (e->key() == Qt::Key_R)
        {
            solver->autoSolve(*field, false, true, false);

            // check if player has won:
            if (field->flagsCount + field->countUnrevealed == field->mines)
            {
                Common::Coords dummyCoords;
                field->gameOver(dummyCoords, false);
            }
        }
        else if (e->key() == Qt::Key_S)
        {
            solver->autoSolve(*field, true, true, true);

            // check if player has won:
            if (field->flagsCount + field->countUnrevealed == field->mines)
            {
                Common::Coords dummyCoords;
                field->gameOver(dummyCoords, false);
            }
        }
    }
}
