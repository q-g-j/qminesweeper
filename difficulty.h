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
    void on_button_start_clicked();
    void on_button_cancel_clicked();

signals:
    void button_clicked_signal(const Difficulty::DifficultyStruct&);
};

#endif // DIFFICULTY_H
