#include "difficulty.h"
#include "ui_difficulty.h"

#include "field.h"
#include "keepsquareandcentered.h"
#include "mainwindow.h"

#include <QMessageBox>

Difficulty::Difficulty(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Difficulty)
{
    ui->setupUi(this);
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
    diff.cols = 20;
    diff.rows = 20;
    diff.mines = 99;
    emit buttonClicked(diff);
}

void Difficulty::on_button_custom_clicked()
{
    if (ui->lineEdit_custom_size->text() == "" || ui->lineEdit_custom_mines->text() == "")
        QMessageBox::warning(this, "Error", "Wrong input, try again..");
}
