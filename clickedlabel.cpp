#include "clickedlabel.h"

ClickedLabel::ClickedLabel(QWidget *parent):
    QLabel(parent)
{

}

void ClickedLabel::mouseReleaseEvent(QMouseEvent *evt)
{
    Q_UNUSED(evt);
    emit clicked();
}
