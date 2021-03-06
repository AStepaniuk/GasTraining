#include "mainview.h"

#include <QPainter>
#include <QMouseEvent>
//#include <optional>
#include <limits>
#include <cmath>
#include <algorithm>

namespace MainViewImpl
{
    constexpr auto MouseSensitivity = 15;
}

using namespace MainViewImpl;

MainView::MainView(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
}

void MainView::setModel(const Model *model)
{
    this->model = model;
    viewPoints = toViewCoordinates(model->getNodes(), QPointF{1.0, 1.0}, QPointF{0.0, 0.0});
    adjustCoordinates(&viewPoints);
}

void MainView::markPicketSucceed(size_t picketIndex)
{
    const auto alreadySuccededIt = std::find(succeedPickets.begin(), succeedPickets.end(), picketIndex);
    if (alreadySuccededIt == succeedPickets.end())
    {
        succeedPickets.push_back(picketIndex);

        if (hoveredPicket == picketIndex)
        {
            hoveredPicket = -1;
        }

        update();
    }
}

void MainView::stopInteractivety()
{
    if (hoveredPicket >= 0)
    {
        hoveredPicket = -1;
        update();
    }

    interactive = false;
}

void MainView::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen{ QColor { 255, 255, 255 } });

    painter.setBrush(QBrush { QColor { 255, 255, 255 } });
    painter.drawRect(0, 0, width(), height());

    painter.setPen(QPen{ QBrush { QColor { 0, 0, 0 } }, 3.0 });


    QPoint prev;
    auto first = true;
    for(const auto& view : viewPoints)
    {
        const QPoint current{ static_cast<int>(view.x()), static_cast<int>(view.y()) };

        if (!first)
        {
            painter.drawLine(prev, current);
        }

        prev = current;
        first = false;
    }

    painter.setPen(QPen{ QBrush { QColor { 0, 0, 0 } }, 2.0 });

    const auto& nodes = model->getNodes();
    for(size_t i = 0; i < nodes.size(); ++i)
    {
        const auto& node = nodes[i];
        if (node->type == NodeType::End)
        {
            if (i == 0)
            {
                drawPipeEnd(&painter, viewPoints[0], viewPoints[1]);
            }
            else
            {
                drawPipeEnd(&painter, viewPoints[i], viewPoints[i-1]);
            }
        }
    }

    if (hoveredPicket >= 0)
    {
        painter.setPen(QPen{ QBrush { QColor { 70, 70, 200 } }, 2.0 });
        painter.setBrush(Qt::NoBrush);

        const auto& point = getNodeViewPointByPicketIndex(static_cast<size_t>(hoveredPicket));

        painter.drawRect(point.x() - MouseSensitivity, point.y() - MouseSensitivity, MouseSensitivity*2, MouseSensitivity*2);
    }

    // succedded pickets
    if (!succeedPickets.empty())
    {
        painter.setPen(QPen{ QBrush { QColor { 20, 180, 20 } }, 2.0 });
        painter.setBrush(Qt::NoBrush);

        for(const auto picketIndex : succeedPickets)
        {
            const auto& point = getNodeViewPointByPicketIndex(static_cast<size_t>(picketIndex));
            painter.drawEllipse(point, MouseSensitivity, MouseSensitivity);
        }
    }
}

void MainView::resizeEvent(QResizeEvent *)
{
    viewPoints = toViewCoordinates(model->getNodes(), QPointF{1.0, 1.0}, QPointF{0.0, 0.0});
    adjustCoordinates(&viewPoints);
}

void MainView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::MouseButton::LeftButton)
    {
        return;
    }

    if (hoveredPicket >= 0)
    {
        emit picketClicked(static_cast<size_t>(hoveredPicket));
    }
}

void MainView::mouseMoveEvent(QMouseEvent *event)
{
    if (!interactive)
    {
        return;
    }

    const auto nodeIndex = getNodeUnderPosition(QPoint(event->x(), event->y()), MouseSensitivity);

    auto picketIndex = -1;
    if (nodeIndex >= 0)
    {
        const auto node = model->getNodes()[nodeIndex];
        picketIndex = findPicketForNode(node);
    }

    if (picketIndex != -1)
    {
        auto picketSuccededIt = std::find(succeedPickets.begin(), succeedPickets.end(), picketIndex);
        if (picketSuccededIt != succeedPickets.end())
        {
            picketIndex = -1;
        }
    }

    if (picketIndex != hoveredPicket)
    {
        hoveredPicket = picketIndex;
        update();
    }
}

std::vector<QPointF> MainView::toViewCoordinates(const std::vector<Node*> &nodes, QPointF scale, QPointF offset) const
{
    std::vector<QPointF> points{};
    points.reserve(nodes.size());

    for(const auto& node : nodes)
    {
        points.push_back(toViewCoopdinates(node, scale, offset));
    }

    return points;
}

QPointF MainView::toViewCoopdinates(const Node* node, QPointF scale, QPointF offset) const
{
    return {
        (node->x + node->y * 0.707 * 0.5) * scale.x() + offset.x(),
        (node->y * 0.707 * 0.5 + node->z) * scale.y() + offset.y()
    };
}

void MainView::adjustCoordinates(std::vector<QPointF> *points) const
{
    QPointF min{ std::numeric_limits<qreal>::max(),  std::numeric_limits<qreal>::max() };
    QPointF max{ -std::numeric_limits<qreal>::max(),  -std::numeric_limits<qreal>::max() };

    for(const auto& view : *points)
    {
        if (view.x() < min.x())
        {
            min.setX(view.x());
        }
        if (view.y() < min.y())
        {
            min.setY(view.y());
        }

        if (view.x() > max.x())
        {
            max.setX(view.x());
        }
        if (view.y() > max.y())
        {
            max.setY(view.y());
        }
    }

    const auto d = max - min;
    const QPointF sp { width() / d.x() * 0.9, height() / d.y() * 0.9 };
    const auto s = sp.x() < sp.y() ? sp.x() : sp.y();

    const QPointF o { (width() - d.x() * s) * 0.5 - min.x() * s, (height() - d.y() * s) * 0.5 - min.y() * s };

    for(auto& view : *points)
    {
        view.setX(view.x() * s + o.x());
        view.setY(view.y() * s + o.y());
    }
}

QPointF MainView::angle(QPointF point, QPointF direction) const
{
    const auto d = direction - point;
    const auto dist = std::sqrt(d.x() * d.x() + d.y() * d.y());

    return QPointF { d.x() / dist, d.y() / dist };
}

void MainView::drawLine(QPainter* painter, QPointF base, QPointF angle, const std::initializer_list<QPointF> &points)
{
    QPoint prev;
    auto first = true;
    for(const auto& point : points)
    {
        const QPoint current{
            static_cast<int>(base.x() + point.x() * angle.x() - point.y() * angle.y()),
            static_cast<int>(base.y() + point.x() * angle.y() + point.y() * angle.x())
        };

        if (!first)
        {
            painter->drawLine(prev, current);
        }

        prev = current;
        first = false;
    }
}

void MainView::drawPipeEnd(QPainter* painter, QPointF point, QPointF direction)
{
    const auto a = angle(point, direction);

    drawLine(painter, point, a, {
        QPointF { 5.0, -10.0 },
        QPointF { 0.0, -10.0 },
        QPointF { 0.0, 10.0 },
        QPointF { 5.0, 10.0 }
             });
}

QPointF MainView::getNodeViewPointByPicketIndex(size_t picketIndex)
{
    const auto& picket = model->getPickets()[picketIndex];

    const auto& nodes = model->getNodes();
    const auto nodeIt = std::find(nodes.begin(), nodes.end(), picket.node);
    const auto nodeIndex = nodeIt - nodes.begin();

    return viewPoints[static_cast<size_t>(nodeIndex)];
}

int MainView::getNodeUnderPosition(QPoint position, int d)
{
    std::vector<size_t> candidates;

    for(size_t i = 0; i < viewPoints.size(); ++i)
    {
        const auto& point = viewPoints[i];
        if (
            point.x() - d < position.x() && point.x() + d > position.x() &&
            point.y() - d < position.y() && point.y() + d > position.y()
        )
        {
            candidates.push_back(i);
        }
    }

    if (candidates.size() == 0)
    {
        return -1;
    }
    else if (candidates.size() == 1)
    {
        return static_cast<int>(candidates[0]);
    }
    else
    {
        auto bestCandidate = -1;
        auto bestDistance = 0.0;

        for(const auto i : candidates)
        {
            const auto& point = viewPoints[i];

            const auto dx = position.x() - point.x();
            const auto dy = position.y() - point.y();
            auto distance = dx*dx + dy*dy;

            if (bestCandidate < 0 || bestDistance > distance)
            {
                bestCandidate = static_cast<int>(i);
                bestDistance = distance;
            }
        }

        return bestCandidate;
    }
}

int MainView::findPicketForNode(const Node *node)
{
    const auto& pickets = model->getPickets();
    const auto picketIt = std::find_if(pickets.begin(), pickets.end(), [node](const auto& p) { return p.node == node; });

    return picketIt != pickets.end()
        ? picketIt - pickets.begin()
        : -1;
}
