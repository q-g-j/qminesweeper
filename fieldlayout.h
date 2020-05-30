#ifndef FIELDLAYOUT_H
#define FIELDLAYOUT_H

#include <QGridLayout>
#include <QSize>
#include <QResizeEvent>
 
class FieldLayout : public QGridLayout
{
Q_OBJECT
public:
    explicit FieldLayout(QWidget *parent = nullptr, int const& cellSize = 0);
 
signals:
 
public slots:
    void onColsChanged(int const&);
    void onRowsChanged(int const&);
 
protected:
    void setGeometry(const QRect& oldRect) override;
 
private:
    QRect newRect;
    bool colsChanged = false;

    int cols;
    int rows;
    int cellSize;
}; 

#endif // FIELDLAYOUT_H
