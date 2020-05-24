#include "keepsquareandcentered.h"

void KeepSquareAndCentered::set_widget (QWidget *widget)
{
  if ((m_widget = widget))
    m_widget->setParent(this);
}

QSize KeepSquareAndCentered::sizeHint () const
{
  return(m_widget ? m_widget->sizeHint() : super::sizeHint());
}

void KeepSquareAndCentered::resizeEvent (QResizeEvent *event)
{
  super::resizeEvent(event);
  fixup();
}

void KeepSquareAndCentered::fixup ()
{
  if (m_widget) {
    QRect r(QPoint(), QSize(height(), height()));
    r.moveCenter(rect().center());
    m_widget->setGeometry(r);
  }
}
