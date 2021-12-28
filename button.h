#ifndef BUTTON_H
#define BUTTON_H

#include <QMouseEvent>
#include <QPushButton>

class Button : public QPushButton
{
    Q_OBJECT
private:
    bool leftbuttonpressedflag = false;
public:
    explicit Button(QWidget *parent = nullptr);
protected:
    void mouseDoubleClickEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    bool eventFilter(QObject*, QEvent*);
signals:
    void double_clicked_signal();
    void left_pressed_signal();
    void left_released_signal();
    void right_released_signal();
    void left_pressed_and_moved_signal(QMouseEvent*);
};

#endif // BUTTON_H
