#ifndef BUTTON_H
#define BUTTON_H

#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>

class Button : public QLabel
{
    Q_OBJECT
private:
    bool leftbuttonpressedflag = false;
    bool rightbuttonpressedflag = false;
    bool leftandrightbuttonpressedflag = false;
public:
    explicit Button(QWidget *parent = nullptr);
    ~Button();

    struct ButtonIconsStruct
    {
        QPixmap button_1;
        QPixmap button_2;
        QPixmap button_3;
        QPixmap button_4;
        QPixmap button_5;
        QPixmap button_6;
        QPixmap button_7;
        QPixmap button_8;
        QPixmap button_mine;
        QPixmap button_mine_hit;
        QPixmap button_revealed;
        QPixmap button_unrevealed;
        QPixmap button_pressed;
        QPixmap button_flag;
    };
protected:
    bool eventFilter(QObject*, QEvent*);
signals:
    void double_clicked_signal();
    void left_pressed_signal(QMouseEvent*);
    void right_pressed_signal(QMouseEvent*);
    void left_and_right_pressed_signal(QMouseEvent*);
    void left_released_signal();
    void right_released_signal();
    void left_and_right_released_signal();
    void left_pressed_and_moved_signal(QMouseEvent*);
    void right_pressed_and_moved_signal(QMouseEvent*);
    void left_and_right_pressed_and_moved_signal(QMouseEvent*);

    void print_debug_signal(const QString&);
};

#endif // BUTTON_H
