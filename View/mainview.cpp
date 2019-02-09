#include "mainview.h"

#include <QPainter>
//#include <optional>

MainView::MainView(QWidget *parent) : QWidget(parent)
{

}

void MainView::setModel(const Model *model)
{
    this->model = model;
}

void MainView::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    painter.setPen(QPen{ QColor { 0, 0, 0 } });

    painter.setBrush(QBrush { QColor { 255, 255, 255 } });
    painter.drawRect(0, 0, width(), height());

    QPoint prev;
    for(const auto& node : model->getNodes())
    {
        const auto current = toViewCoopdinates(node);

        if (!prev.isNull())
        {
            painter.drawLine(prev, current);
        }

        prev = current;
    }
}

QPoint MainView::toViewCoopdinates(const Node& node) const
{
    return {
        static_cast<int>(node.x + node.y * 0.707 * 0.5),
        static_cast<int>(node.y * 0.707 * 0.5 + node.z)
    };
}
