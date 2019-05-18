#ifndef PICKETVIEW_H
#define PICKETVIEW_H

#include <QWidget>

#include "Data/model.h"

class PicketView : public QWidget
{
    Q_OBJECT
public:
    explicit PicketView(QWidget *parent = nullptr);

    void setPicket(const Picket& picket);

    void makeActive();
    void makeSuccess();
    void makeFailed();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const Picket* picket;

    enum class Status { Passive, Active, Succeed, Failed };
    Status status;
};

#endif // PICKETVIEW_H
