#include "difficulty.h"
#include "ui_difficulty.h"

#include "field.h"
#include "mainwindow.h"

#include <QMessageBox>

Difficulty::Difficulty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Difficulty)
{
    ui->setupUi(this);
    intValidatorCols = new QIntValidator(5, 30, this);
    intValidatorRows = new QIntValidator(5, 30, this);
    intValidatorMines = new QIntValidator(1, 200, this);
    ui->lineEdit_custom_cols->setValidator(intValidatorCols);
    ui->lineEdit_custom_rows->setValidator(intValidatorRows);
    ui->lineEdit_custom_mines->setValidator(intValidatorMines);
}

Difficulty::~Difficulty()
{
    delete ui;
}

void Difficulty::on_button_small_clicked()
{
    close();
    DifficultyStruct diff;
    diff.cols = 9;
    diff.rows = 9;
    diff.mines = 10;
    emit buttonClicked(diff);
}

void Difficulty::on_button_medium_clicked()
{
    close();
    DifficultyStruct diff;
    diff.cols = 16;
    diff.rows = 16;
    diff.mines = 40;
    emit buttonClicked(diff);
}

void Difficulty::on_button_large_clicked()
{
    close();
    DifficultyStruct diff;
    diff.cols = 30;
    diff.rows = 16;
    diff.mines = 99;
    emit buttonClicked(diff);
}

void Difficulty::on_button_custom_clicked()
{
    int cols = ui->lineEdit_custom_cols->text().toInt();
    int rows = ui->lineEdit_custom_rows->text().toInt();
    int mines = ui->lineEdit_custom_mines->text().toInt();
    if (ui->lineEdit_custom_cols->text() == "" || ui->lineEdit_custom_rows->text() == "" || ui->lineEdit_custom_mines->text() == "" || mines > (cols * rows - 1))
        QMessageBox::warning(this, "Error", "Wrong input, try again..");
    else
    {
        close();
        DifficultyStruct diff;
        diff.cols = cols;
        diff.rows = rows;
        diff.mines = mines;
        emit buttonClicked(diff);
    }
}
