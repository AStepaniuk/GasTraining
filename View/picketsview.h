#ifndef PICKETSVIEW_H
#define PICKETSVIEW_H

#include <QWidget>
#include <QHBoxLayout>

#include "Layouts/flowlayout.h"
#include "Data/model.h"

class PicketsView : public QWidget
{
    Q_OBJECT
public:
    explicit PicketsView(QWidget *parent = 0);

    void setPickets(const std::vector<Picket>& pickets);

signals:

public slots:

private:
    FlowLayout *picketsLayout;
};

#endif // PICKETSVIEW_H
