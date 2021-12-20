#include <QDebug>
#include <QSizePolicy>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>

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

    labelMinesLeftWrapperWidth = ui->labelMinesLeftWrapper->width();
    labelMinesLeftWrapperHeight = ui->labelMinesLeftWrapper->height();
    timerWrapperHeight = ui->timerWrapper->height();
    minesLeftNumberWidth = ui->labelMinesLeftOnes->width();
    spacerMiddleLeftFixedWidth = 8;

    // width and height of a cell in pixels:
    this->cellSize = 25;

    this->fieldLayout = new QGridLayout(ui->fieldWrapper);

    QFontDatabase::addApplicationFont(":/fonts/NotoSans-CondensedMedium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/digital-7.ttf");

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
    if (difficulty_.mines < 100)
    {
        ui->labelMinesLeftTens->show();
        ui->labelMinesLeftOnes->show();
        ui->labelMinesLeftThousands->hide();
        ui->labelMinesLeftHundreds->hide();
        ui->labelMinesLeftWrapper->resize(labelMinesLeftWrapperWidth - 2*minesLeftNumberWidth, labelMinesLeftWrapperHeight);
        ui->spacerMiddleLeftFixed->changeSize(spacerMiddleLeftFixedWidth + 2 * minesLeftNumberWidth, labelMinesLeftWrapperHeight);
    }
    else if (difficulty_.mines < 1000)
    {
        ui->labelMinesLeftHundreds->show();
        ui->labelMinesLeftTens->show();
        ui->labelMinesLeftOnes->show();
        ui->labelMinesLeftThousands->hide();
        ui->labelMinesLeftWrapper->resize(labelMinesLeftWrapperWidth - minesLeftNumberWidth, labelMinesLeftWrapperHeight);
        ui->spacerMiddleLeftFixed->changeSize(spacerMiddleLeftFixedWidth + minesLeftNumberWidth, labelMinesLeftWrapperHeight);
    }
    else
    {
        ui->labelMinesLeftThousands->show();
        ui->labelMinesLeftHundreds->show();
        ui->labelMinesLeftTens->show();
        ui->labelMinesLeftOnes->show();
        ui->labelMinesLeftWrapper->resize(labelMinesLeftWrapperWidth, labelMinesLeftWrapperHeight);
        ui->spacerMiddleLeftFixed->changeSize(spacerMiddleLeftFixedWidth, labelMinesLeftWrapperHeight);
    }

    // if changing the size of a spacer, need to invalidate its layout:
    ui->infoBarLayout->invalidate();

    ui->timerWrapper->adjustSize();
    ui->labelMinesLeftWrapper->adjustSize();

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
    this->minesleft_changed_slot(difficulty_.mines);
    connect(this->field, &Field::game_over_signal, this, &MainWindow::game_over_slot);
    connect(this->field, &Field::minesleft_changed_signal, this, &MainWindow::minesleft_changed_slot);
    connect(this->field, &Field::smiley_surprised_signal, this, &MainWindow::smiley_surprised_slot);
}

// open a dialog (difficulty.ui) to choose difficulty:
void MainWindow::on_actionNew_triggered()
{
    Difficulty difficulty_(this);
    connect(&difficulty_, &Difficulty::button_clicked_signal, this, &MainWindow::new_game_slot);
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

void MainWindow::smiley_surprised_slot()
{
    if (field->isGameOver != true)
    {
        ui->smiley->setStyleSheet(this->stylesheet_smiley_surprised);
        Common::sleep(350);
        if (field->isGameOver != true)
            ui->smiley->setStyleSheet(this->stylesheet_smiley);
    }
}

void MainWindow::game_over_slot(const QString& mode)
{
    timer->timerInstance->stop();
    if (mode == "lose")
        ui->smiley->setStyleSheet(this->stylesheet_smiley_lost);
    else if (mode == "win")
        ui->smiley->setStyleSheet(this->stylesheet_smiley_won);
}

void MainWindow::minesleft_changed_slot(const int& minesLeft)
{
    if (minesLeft < 0)
        ui->labelMinesLeftOnes->setText("0");
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
