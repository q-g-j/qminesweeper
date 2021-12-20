#ifndef CELL_H
#define CELL_H

#include <QMouseEvent>
#include <QPushButton>

class Cell : public QPushButton
{
    Q_OBJECT
private:
public:
    explicit Cell(QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
signals:
    void double_clicked_signal();
    void left_released_signal();
    void right_released_signal();
};

#endif // CELL_H
