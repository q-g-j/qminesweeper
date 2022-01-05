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
    QValidator *intValidatorCols = nullptr;
    QValidator *intValidatorRows = nullptr;
    QValidator *intValidatorMines = nullptr;
public:
    explicit Difficulty(QWidget *parent = nullptr);
    ~Difficulty();
    Ui::Difficulty *ui;

    // start in easy mode:
    struct DifficultyStruct {
    quint16 cols = 70;
    quint16 rows = 35;
    quint16 mines = 200;
    };
private slots:
    void on_button_start_clicked();
    void on_button_cancel_clicked();
signals:
    void button_clicked_signal(const Difficulty::DifficultyStruct&);
};

#endif // DIFFICULTY_H
