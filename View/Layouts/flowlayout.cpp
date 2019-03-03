#include <QtWidgets>

#include "flowlayout.h"

FlowLayout::FlowLayout(QWidget *parent)
    : QLayout(parent)
{
}

FlowLayout::FlowLayout()
{
}

FlowLayout::~FlowLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

int FlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    else
        return nullptr;
}

void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    QLayoutItem *item;
    foreach (item, itemList)
        size = size.expandedTo(item->minimumSize());

    size += QSize(2*margin(), 2*margin());
    return size;
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();

    QSize picketSize(static_cast<int>(effectiveRect.height() / 200.0 * 140.0), effectiveRect.height());

    QLayoutItem *item;
    foreach (item, itemList) {
        QWidget *wid = item->widget();
        int space = spacing();
        if (space == -1)
            space = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);

        int nextX = x + picketSize.width() + space;
        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), picketSize));

        x = nextX;
    }

    return 0;
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    } else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}
