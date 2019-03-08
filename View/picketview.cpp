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

    const auto node = picket->node;
    const auto section = node->section;
    const auto pipeline = section->pipeline;

    if (section->material == PipeMaterial::Pe)
    {
        painter.setBrush(QBrush { QColor { 255, 245, 0 } });
    }
    else
    {
        painter.setBrush(QBrush { QColor { 0, 126, 82 } });
    }
    painter.drawRect(0, 0, width()-1, height()-1);
}
