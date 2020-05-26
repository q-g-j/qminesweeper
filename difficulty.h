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
    void on_button_small_clicked();
    void on_button_medium_clicked();
    void on_button_large_clicked();
    void on_button_custom_clicked();

signals:
    void buttonClicked(Difficulty::DifficultyStruct const&);
};

#endif // DIFFICULTY_H
