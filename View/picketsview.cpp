#include "picketsview.h"

#include <QScrollArea>
#include <QtAlgorithms>

#include "picketview.h"

PicketsView::PicketsView(QWidget *parent) : QWidget(parent)
{
    picketsLayout = new FlowLayout();
    picketsLayout->setMargin(0);
    picketsLayout->setSpacing(3);
    // picketsLayout->

    auto scrollArea = new QScrollArea();
    scrollArea->setLayout(picketsLayout);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);

    auto mainLayout = new QHBoxLayout();
    mainLayout->addWidget(scrollArea);
    mainLayout->setMargin(0);

    setLayout(mainLayout);
}

void PicketsView::setPickets(const std::vector<Picket> &pickets)
{
    qDeleteAll(picketsLayout->children());

    this->pickets = pickets;
}

void PicketsView::setActivePicket(size_t index)
{
    const auto viewIterator = views.find(index);
    if (viewIterator == views.end())
    {
        const auto& picket = pickets[index];
        auto widget = new PicketView();
        widget->setPicket(picket);

        picketsLayout->addWidget(widget);
        views[index] = widget;

        widget->makeActive();
    }
    else
    {
        viewIterator->second->makeActive();
    }
}

void PicketsView::markSucceed(size_t index)
{
    views[index]->makeSuccess();
}

void PicketsView::markFailed(size_t index)
{
    views[index]->makeFailed();
}
