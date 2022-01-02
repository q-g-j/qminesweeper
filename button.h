#ifndef BUTTON_H
#define BUTTON_H

#include <QMouseEvent>
#include <QPushButton>

class Button : public QPushButton
{
    Q_OBJECT
private:
    bool leftbuttonpressedflag = false;
    bool rightbuttonpressedflag = false;
    bool leftandrightbuttonpressedflag = false;
public:
    explicit Button(QWidget *parent = nullptr);
    ~Button();
protected:
    bool eventFilter(QObject*, QEvent*);
signals:
    void double_clicked_signal();
    void left_pressed_signal();
    void right_pressed_signal();
    void left_released_signal();
    void right_released_signal();
    void left_and_right_pressed_signal();
    void left_and_right_released_signal();
    void left_pressed_and_moved_signal(QMouseEvent*);
    void right_pressed_and_moved_signal(QMouseEvent*);
    void left_and_right_pressed_and_moved_signal(QMouseEvent*, bool);

    void print_debug_signal(const QString&);
};

#endif // BUTTON_H
