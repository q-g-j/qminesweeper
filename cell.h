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
    void doubleClicked();
    void leftReleased();
    void rightReleased();
};

#endif // CELL_H
