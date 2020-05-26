#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include <QDialog>
#include <QValidator>

namespace Ui {
class Difficulty;
}

class Difficulty : public QDialog
{
    Q_OBJECT    
private:
    QValidator *intValidatorCols;
    QValidator *intValidatorRows;
    QValidator *intValidatorMines;
public:
    explicit Difficulty(QWidget *parent = nullptr);
    ~Difficulty();
    Ui::Difficulty *ui;

    struct DifficultyStruct {
        int cols;
        int rows;
        int mines;
    };

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

signals:
    void buttonClicked(Difficulty::DifficultyStruct const&);
};

#endif // DIFFICULTY_H
