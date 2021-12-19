#include <QDebug>
#include <QMessageBox>

#include "difficulty.h"
#include "ui_difficulty.h"

Difficulty::Difficulty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Difficulty)
{
    ui->setupUi(this);
    intValidatorCols = new QIntValidator(9, 60, this);
    intValidatorRows = new QIntValidator(9, 30, this);
    intValidatorMines = new QIntValidator(1, 9999, this);
    ui->lineEdit_custom_cols->setValidator(intValidatorCols);
    ui->lineEdit_custom_rows->setValidator(intValidatorRows);
    ui->lineEdit_custom_mines->setValidator(intValidatorMines);
    ui->radioButton_easy->setChecked(true);
    ui->radioButton_easy->setFocus();
    this->setFixedSize(this->size().width(), this->size().height());
}

Difficulty::~Difficulty()
{
    delete ui;
}

void Difficulty::on_button_start_clicked()
{
    if (ui->radioButton_easy->isChecked())
    {
        close();
        DifficultyStruct diff;
        diff.cols = 9;
        diff.rows = 9;
        diff.mines = 10;
        emit buttonClicked(diff);
    }

    else if (ui->radioButton_advanced->isChecked())
    {
        close();
        DifficultyStruct diff;
        diff.cols = 16;
        diff.rows = 16;
        diff.mines = 40;
        emit buttonClicked(diff);
    }

    else if (ui->radioButton_expert->isChecked())
    {
        close();
        DifficultyStruct diff;
        diff.cols = 30;
        diff.rows = 16;
        diff.mines = 99;
        emit buttonClicked(diff);
    }

    else if (ui->radioButton_custom->isChecked())
    {
        int cols = ui->lineEdit_custom_cols->text().toInt();
        int rows = ui->lineEdit_custom_rows->text().toInt();
        int mines = ui->lineEdit_custom_mines->text().toInt();
        if (ui->lineEdit_custom_cols->text() == ""
                || ui->lineEdit_custom_rows->text() == ""
                || ui->lineEdit_custom_mines->text() == ""
                || cols > 60
                || rows > 30
                || cols < 9
                || rows < 9
                || mines > (cols * rows - 1)
                || mines < 1)
        {
            QString msg = "Wrong input, try again. Valid numbers are:\n\n"
                    "columns:\n"
                    "   9 to 60\n"
                    "rows:\n"
                    "   9 to 30\n"
                    "mines:\n"
                    "   1 to (columns * rows - 1)";
            QMessageBox::warning(this, "Error", msg);
        }
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

}

void Difficulty::on_button_cancel_clicked()
{
    close();
}
