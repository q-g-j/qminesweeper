#ifndef FIELDLAYOUT_H
#define FIELDLAYOUT_H

#include <QGridLayout>
#include <QSize>
#include <QResizeEvent>
 
class FieldLayout : public QGridLayout
{
Q_OBJECT
public:
    explicit FieldLayout(QWidget *parent = nullptr);
 
signals:
 
public slots:
    void onColsChanged(int const&);
    void onRowsChanged(int const&);
 
//protected:
    void setGeometry(const QRect& oldRect);
 
private:
    int cols;
    int rows;

    bool widthChanged = false;
    bool heightChanged = false;
    
    int oldWidth;
    int oldHeight;
    
    int widthNoHeightChange;
    int heightNoWidthChange;
    
    
};
 

#endif // FIELDLAYOUT_H
