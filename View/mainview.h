#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>

#include "Data/model.h"

class MainView : public QWidget
{
    Q_OBJECT
public:
    explicit MainView(QWidget *parent = nullptr);

    void setModel(const Model* model);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const Model* model;

    QPoint toViewCoopdinates(const Node& node) const;
};

#endif // MAINVIEW_H
