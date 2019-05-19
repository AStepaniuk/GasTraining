#ifndef PICKETSVIEW_H
#define PICKETSVIEW_H

#include <QWidget>
#include <QHBoxLayout>

#include <vector>

#include "Layouts/flowlayout.h"
#include "Data/model.h"

#include "picketview.h"

class PicketsView : public QWidget
{
    Q_OBJECT
public:
    explicit PicketsView(QWidget *parent = 0);

    void setPickets(const std::vector<Picket>& pickets);
    void setActivePicket(size_t index);
    void markSucceed(size_t index);
    void markFailed(size_t index);

signals:

public slots:

private:
    FlowLayout *picketsLayout;

    std::vector<PicketView*> views;
};

#endif // PICKETSVIEW_H
