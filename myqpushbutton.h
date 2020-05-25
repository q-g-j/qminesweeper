#ifndef MYQPUSHBUTTON_H
#define MYQPUSHBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class MyQPushButton : public QPushButton
{
    Q_OBJECT
private:
public:
    explicit MyQPushButton(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    //    void mouseMoveEvent(QMouseEvent *e);
signals:
    void leftPressed();
    void rightPressed();
    void leftReleased();
    void rightReleased();
};

#endif // MYQPUSHBUTTON_H
