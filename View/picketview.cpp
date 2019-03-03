#include "picketview.h"

#include <QPainter>
#include <QResizeEvent>

PicketView::PicketView(QWidget *parent) : QWidget(parent)
{

}

void PicketView::setPicket(const Picket &picket)
{
    this->picket = &picket;

    update();
}

void PicketView::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    painter.setPen(QPen{ QColor { 0, 0, 0 } });

    painter.setBrush(QBrush { QColor { 255, 255, 255 } });
    painter.drawRect(0, 0, width()-1, height()-1);
}
