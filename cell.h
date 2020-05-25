#ifndef MYQPUSHBUTTON_H
#define MYQPUSHBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class Cell : public QPushButton
{
    Q_OBJECT
private:
public:
    explicit Cell(QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    //    void mouseMoveEvent(QMouseEvent *e);
signals:
    void doubleClicked();
    void leftPressed();
    void rightPressed();
    void leftReleased();
    void rightReleased();
};

#endif // MYQPUSHBUTTON_H
