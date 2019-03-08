#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>

#include "Data/model.h"
#include <initializer_list>

class MainView : public QWidget
{
    Q_OBJECT
public:
    explicit MainView(QWidget *parent = nullptr);

    void setModel(const Model* model);

signals:
    void nodeClicked(size_t nodeIndex);

public slots:

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const Model* model;
    std::vector<QPointF> viewPoints;

    std::vector<QPointF> toViewCoordinates(const std::vector<Node*>& nodes, QPointF scale, QPointF offset) const;
    QPointF toViewCoopdinates(const Node* node, QPointF scale, QPointF offset) const;

    void adjustCoordinates(std::vector<QPointF>* points) const;

    QPointF angle(QPointF point, QPointF direction) const;
    void drawLine(QPainter* painter, QPointF base, QPointF angle, const std::initializer_list<QPointF>& points);
    void drawPipeEnd(QPainter* painter, QPointF point, QPointF direction);
};

#endif // MAINVIEW_H
