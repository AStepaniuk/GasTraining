#ifndef PICKETSVIEW_H
#define PICKETSVIEW_H

#include <QWidget>
#include <QHBoxLayout>

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
    QHBoxLayout *picketsLayout;
};

#endif // PICKETSVIEW_H
