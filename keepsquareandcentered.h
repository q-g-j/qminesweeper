#ifndef KEEPSQUAREANDCENTERED_H
#define KEEPSQUAREANDCENTERED_H

#include <QWidget>

class KeepSquareAndCentered: public QWidget
{
    using super = QWidget;
public:
    explicit KeepSquareAndCentered (QWidget *parent = nullptr)
    :
      super(parent),
      m_widget(nullptr)
    {
        //
    }
    virtual void set_widget (QWidget *widget);
    virtual QSize sizeHint () const override;
protected:
    virtual void resizeEvent (QResizeEvent *event) override;
private:
    QWidget *m_widget;
    void fixup ();
};

#endif // KEEPSQUAREANDCENTERED_H
