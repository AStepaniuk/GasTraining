#include "picketsview.h"

#include <QScrollArea>
#include <QtAlgorithms>

#include "picketview.h"

PicketsView::PicketsView(QWidget *parent) : QWidget(parent)
{
    picketsLayout = new QHBoxLayout();
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

    for(const auto& picket : pickets)
    {
        auto widget = new PicketView();
        widget->setPicket(picket);

        picketsLayout->addWidget(widget);
    }
}
