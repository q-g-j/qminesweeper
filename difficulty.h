#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include <QWidget>

namespace Ui {
class Difficulty;
}

class Difficulty : public QWidget
{
    Q_OBJECT    
private:
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
    void buttonClicked(DifficultyStruct const&);
};

typedef struct Difficulty::DifficultyStruct DifficultyStruct;

#endif // DIFFICULTY_H
