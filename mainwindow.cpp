#include <QDebug>
#include <QSizePolicy>
#include <QStyle>
#include <QFrame>
#include <QScreen>
#include <QThread>
//#include <QFontDatabase>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sleep.h"
#include "timer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->installEventFilter(this);

    this->buttonIcons.button_1.load(":/icons/png/button_1.png");
    this->buttonIcons.button_2.load(":/icons/png/button_2.png");
    this->buttonIcons.button_3.load(":/icons/png/button_3.png");
    this->buttonIcons.button_4.load(":/icons/png/button_4.png");
    this->buttonIcons.button_5.load(":/icons/png/button_5.png");
    this->buttonIcons.button_6.load(":/icons/png/button_6.png");
    this->buttonIcons.button_7.load(":/icons/png/button_7.png");
    this->buttonIcons.button_8.load(":/icons/png/button_8.png");
    this->buttonIcons.button_revealed.load(":/icons/png/button_revealed.png");
    this->buttonIcons.button_unrevealed.load(":/icons/png/button_unrevealed.png");
    this->buttonIcons.button_pressed.load(":/icons/png/button_pressed.png");
    this->buttonIcons.button_mine.load(":/icons/png/button_mine.png");
    this->buttonIcons.button_mine_hit.load(":/icons/png/button_mine_hit.png");
    this->buttonIcons.button_flag.load(":/icons/png/button_flag.png");

    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->setFocus();

    this->minesLeftFrameWidth = ui->minesLeftFrame->width();
    this->minesLeftFrameHeight = ui->minesLeftFrame->height();
    this->timerFrameHeight = ui->timerFrame->height();
    this->minesLeftNumberWidth = ui->minesLeftOnes->width();

    this->fieldLayout = new QGridLayout(ui->fieldWrapper);

    this->newGame(this->difficulty);

// on macOS the QFrame border is not displayed correctly, so disable it:
#ifdef __APPLE__
    ui->minesLeftFrame->setFrameShape(QFrame::NoFrame);
    ui->timerFrame->setFrameShape(QFrame::NoFrame);
#endif

    /*
    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/font.ttf");

    // get the font names from the font filenames:
    for (quint16 i = 0; i < fontDatabase.families(QFontDatabase::Any).size(); i++)
    {
        QStringList fontlist = fontDatabase.applicationFontFamilies(i);
        for (quint16 j = 0; j < fontlist.size(); j++)
        {
            qDebug() << fontlist.at(j);
        }
    }
    */
}

MainWindow::~MainWindow()
{
    delete ui;
    if (this->field != nullptr)
    {
        delete this->field;
        this->field = nullptr;
    }
    if (this->mouseInput != nullptr)
    {
        delete this->mouseInput;
        this->mouseInput = nullptr;
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
    delete this->fieldLayout;
    this->fieldLayout = nullptr;
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
    if (this->mouseInput != nullptr)
    {
        delete this->mouseInput;
        this->mouseInput = nullptr;
    }
    if (this->field != nullptr)
    {
        delete this->field;
        this->field = nullptr;
    }
    if (this->solver != nullptr)
    {
        delete this->solver;
        this->solver = nullptr;
    }
    if (this->timer != nullptr)
    {
        delete this->timer;
        this->timer = nullptr;
    }

    this->solver = new Solver;
    this->timer = new Timer;
#ifdef DEBUG_TIMER_START_EARLY
    timer->timerStart();
#endif
    this->field = new Field(
                ui->fieldWrapper,
                this->buttonIcons,
                difficulty_.cols,
                difficulty_.rows,
                difficulty_.mines,
                this->buttonSize
                );
    this->mouseInput = new MouseInput(
                field,
                this->buttonIcons
                );

    connect(this->field, &Field::connect_button_signal, mouseInput, &MouseInput::connect_button_slot);
    connect(this->field, &Field::game_over_signal, this, &MainWindow::game_over_slot);
    connect(this->field, &Field::minesleft_changed_signal, this, &MainWindow::minesleft_changed_slot);
    connect(this->field, &Field::smiley_surprised_signal, this, &MainWindow::smiley_surprised_slot);
    connect(this->field, &Field::game_started_signal, this, &MainWindow::start_timer_slot);
    connect(this->mouseInput, &MouseInput::smiley_surprised_signal, this, &MainWindow::smiley_surprised_slot);
    connect(this->solver, &Solver::solver_stopped_signal, this, &MainWindow::solver_stopped_slot);
    connect(this->solver, &Solver::solver_place_flag_signal, field, &Field::solver_place_flag_slot);
    connect(this->solver, &Solver::is_solver_running_signal, field, &Field::is_solver_running_slot);
    connect(this->solver, &Solver::is_solver_running_signal, mouseInput, &MouseInput::is_solver_running_slot);
    connect(this->timer, &Timer::current_timer_signal, this, &MainWindow::current_timer_slot);

#ifdef DEBUG_MESSAGES
    connect(this->timer, &Timer::current_timer_signal, &Common::current_timer_slot);
    connect(this, &MainWindow::print_debug_signal, &Common::print_debug_slot);
    connect(this->mouseInput, &MouseInput::print_debug_signal, &Common::print_debug_slot);
    connect(this->field, &Field::print_debug_signal, &Common::print_debug_slot);
    connect(this->solver, &Solver::print_debug_signal, &Common::print_debug_slot);
    connect(this->timer, &Timer::print_debug_signal, &Common::print_debug_slot);
#endif

    field->create2DVectors();

    this->clearLayout(this->fieldLayout);
    this->fieldLayout->addWidget(this->field);
    this->fieldLayout->setSpacing(0);
    this->fieldLayout->setContentsMargins(0,0,0,0);
    ui->fieldWrapper->setLayout(this->fieldLayout);
    ui->fieldWrapper->setMinimumSize(
                field->cols * field->buttonSize,
                field->rows * field->buttonSize
                );
    ui->smiley->setStyleSheet(this->stylesheet.smiley);
    this->minesleft_changed_slot(difficulty_.mines);
    this->setInfoBarNumber(ui->timerTenMinutes, 0);
    this->setInfoBarNumber(ui->timerMinutes, 0);
    this->setInfoBarNumber(ui->timerTenSeconds, 0);
    this->setInfoBarNumber(ui->timerSeconds, 0);

    if (difficulty_.mines < 100)
    {
        ui->minesLeftTens->show();
        ui->minesLeftOnes->show();
        ui->minesLeftThousands->hide();
        ui->minesLeftHundreds->hide();
        ui->minesLeftFrame->resize(
                    minesLeftFrameWidth - 2 * minesLeftNumberWidth,
                    minesLeftFrameHeight
                    );
        ui->spacerMiddleLeftFixed->changeSize(
                    spacerMiddleLeftFixedWidth + 2 * minesLeftNumberWidth,
                    minesLeftFrameHeight
                    );
    }
    else if (difficulty_.mines < 1000)
    {
        ui->minesLeftHundreds->show();
        ui->minesLeftTens->show();
        ui->minesLeftOnes->show();
        ui->minesLeftThousands->hide();
        ui->minesLeftFrame->resize(
                    minesLeftFrameWidth - minesLeftNumberWidth,
                    minesLeftFrameHeight
                    );
        ui->spacerMiddleLeftFixed->changeSize(
                    spacerMiddleLeftFixedWidth + minesLeftNumberWidth,
                    minesLeftFrameHeight
                    );
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
    this->centralWidget()->adjustSize();
    this->adjustSize();
    this->setFixedSize(
                this->size().width(),
                this->size().height()
                );
    QList<QScreen*> screens = QGuiApplication::screens();
    for (quint16 i = 0; i < screens.size(); i++)
    {
        this->setGeometry(
                    QStyle::alignedRect(
                        Qt::LeftToRight,
                        Qt::AlignCenter,
                        this->size(),
                        screens[i]->availableGeometry()
                        )
                    );
    }
}

void MainWindow::newGameRequested(const char& from)
{
    if (from == 'm')
    {
        solver->isNewGameRequestedFromMenu = true;
        field->isNewGameRequested = true;
        mouseInput->isNewGameRequested = true;
    }
    else if (from == 's')
    {
        solver->isNewGameRequestedFromSmiley = true;
        field->isNewGameRequested = true;
        mouseInput->isNewGameRequested = true;
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

void MainWindow::setInfoBarNumber(
        QWidget *widget,
        const quint16 &number
        )
{
    if (number == 0)
    {
        widget->setStyleSheet(this->stylesheet.digital_0);
    }
    else if (number == 1)
    {
        widget->setStyleSheet(this->stylesheet.digital_1);
    }
    else if (number == 2)
    {
        widget->setStyleSheet(this->stylesheet.digital_2);
    }
    else if (number == 3)
    {
        widget->setStyleSheet(this->stylesheet.digital_3);
    }
    else if (number == 4)
    {
        widget->setStyleSheet(this->stylesheet.digital_4);
    }
    else if (number == 5)
    {
        widget->setStyleSheet(this->stylesheet.digital_5);
    }
    else if (number == 6)
    {
        widget->setStyleSheet(this->stylesheet.digital_6);
    }
    else if (number == 7)
    {
        widget->setStyleSheet(this->stylesheet.digital_7);
    }
    else if (number == 8)
    {
        widget->setStyleSheet(this->stylesheet.digital_8);
    }
    else if (number == 9)
    {
        widget->setStyleSheet(this->stylesheet.digital_9);
    }
}

// open a dialog (difficulty.ui) to choose difficulty:
void MainWindow::on_actionNew_triggered()
{
    if (solver->isSolverRunning == true)
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
    if (solver->isSolverRunning == true)
    {
        this->isNewGameRequested = true;
        this->newGameRequested('s');
    }
    else
    {
        this->newGameFromSmiley();
    }
}

void MainWindow::change_back_smiley_slot()
{
    if (field->isGameOver != true)
    {
        ui->smiley->setStyleSheet(stylesheet.smiley);
    }
}

void MainWindow::smiley_surprised_slot()
{
    if (field->isGameOver != true)
    {
        ui->smiley->setStyleSheet(stylesheet.smiley_surprised);

        QThread* thread = new QThread();
        Sleep* sleep = new Sleep(thread, 350);
        sleep->moveToThread(thread);
        connect(sleep, &Sleep::finished, this, &MainWindow::change_back_smiley_slot);
    }
}

void MainWindow::game_over_slot(bool hasLost)
{
    mouseInput->isGameOver = true;
    this->timer->timerStop();
    if (hasLost == true)
    {
        ui->smiley->setStyleSheet(this->stylesheet.smiley_lost);
    }
    else if (hasLost == false)
    {
        ui->smiley->setStyleSheet(this->stylesheet.smiley_won);
    }
}

void MainWindow::minesleft_changed_slot(const qint16& minesLeft)
{
    if (minesLeft < 0)
    {
        ui->minesLeftOnes->setStyleSheet(this->stylesheet.digital_0);
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
        quint16 ones = minesLeft % 10;
        quint16 tens = minesLeft / 10;
        this->setInfoBarNumber(ui->minesLeftOnes, ones);
        this->setInfoBarNumber(ui->minesLeftTens, tens);
        ui->minesLeftHundreds->setStyleSheet("QWidget { border-image: none; }");
        ui->minesLeftThousands->setStyleSheet("QWidget { border-image: none; }");
    }
    else if (minesLeft < 1000)
    {
        quint16 ones = minesLeft % 10;
        quint16 tens = (minesLeft % 100) / 10;
        quint16 hundreds = minesLeft / 100;
        this->setInfoBarNumber(ui->minesLeftOnes, ones);
        this->setInfoBarNumber(ui->minesLeftTens, tens);
        this->setInfoBarNumber(ui->minesLeftHundreds, hundreds);
        ui->minesLeftThousands->setStyleSheet("QWidget { border-image: none; }");
    }
    else
    {
        quint16 ones = minesLeft % 10;
        quint16 tens = ((minesLeft % 1000) % 100) / 10;
        quint16 hundreds = (minesLeft % 1000) / 100;
        quint16 thousands = minesLeft / 1000;
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

void MainWindow::current_timer_slot(const Timer::TimerStruct& currentTimer)
{
    this->setInfoBarNumber(ui->timerTenMinutes, currentTimer.tenMinutes);
    this->setInfoBarNumber(ui->timerMinutes, currentTimer.minutes);
    this->setInfoBarNumber(ui->timerTenSeconds, currentTimer.tenSeconds);
    this->setInfoBarNumber(ui->timerSeconds, currentTimer.seconds);
}

bool MainWindow::eventFilter(
        QObject* object,
        QEvent *e
        )
{
    if (object == this && e->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);

        if (field->isSolverRunning != true && field->isGameOver != true)
        {
            if (keyEvent->key() == Qt::Key_F)
            {
                ui->smiley->setStyleSheet(this->stylesheet.smiley);
                solver->autoSolve(*field, true, false, false);
            }
            else if (keyEvent->key() == Qt::Key_R)
            {
                ui->smiley->setStyleSheet(this->stylesheet.smiley);
                solver->autoSolve(*field, false, true, false);

                // check if player has won:
                if (field->flagsCount + field->countUnrevealed == field->mines)
                {
                    Common::Coords dummyCoords;
                    field->gameOver(dummyCoords, false);
                }
            }
            else if (keyEvent->key() == Qt::Key_S)
            {
                ui->smiley->setStyleSheet(this->stylesheet.smiley);
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
    return false;
}
