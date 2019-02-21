#ifndef PICKETVIEW_H
#define PICKETVIEW_H

#include <QWidget>

#include "Data/model.h"

class PicketView : public QWidget
{
    Q_OBJECT
public:
    explicit PicketView(QWidget *parent = 0);

    void setPicket(const Picket& picket);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *) override;

private:
    const Picket* picket;
};

#endif // PICKETVIEW_H
