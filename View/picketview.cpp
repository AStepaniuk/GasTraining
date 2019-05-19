#include "picketview.h"

#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QTransform>

#include <map>
#include <algorithm>
#include <cmath>
#include <functional>
#include <utility>

PicketView::PicketView(QWidget *parent) : QWidget(parent)
{
    status = Status::Passive;
}

void PicketView::setPicket(const Picket &picket)
{
    this->picket = &picket;

    update();
}

const QString& GetPressureSteelTitle(PressureType type)
{
    static std::map<PressureType, QString> pressures
    {
        { PressureType::G1, "НД" },
        { PressureType::G2, "СД" },
        { PressureType::G3, "IК" },
        { PressureType::G4, "IIК" }
    };

    const auto res = pressures.find(type);
    if (res != pressures.end())
    {
        return res->second;
    }

    static QString nothing{};
    return nothing;
}

std::tuple<double, double> GetTurnAngles(const Node& node)
{
    if (node.type != NodeType::Turn)
    {
        return std::make_tuple(0.0, 0.0);
    }

    const auto& nodes = node.section->nodes;
    const auto iter = std::find_if(nodes.begin(), nodes.end(), [&node](const auto& n) { return &n == &node; });

    if (iter == nodes.end())
    {
        return std::make_tuple(0.0, 0.0);
    }

    const size_t i { static_cast<size_t>(iter - nodes.begin()) };
    if (i == 0 || i == nodes.size() - 1)
    {
        return std::make_tuple(0.0, 0.0);
    }

    const auto& prev = nodes[i-1];
    const auto& next = nodes[i+1];

    const auto a1 = std::atan2(prev.y - node.y, prev.x - node.x) * 180.0 / M_PI;
    const auto a2 = std::atan2(next.y - node.y, next.x - node.x) * 180.0 / M_PI;

    return std::make_tuple(a1, a2);
}

int GetNodeAngle(const Node& node)
{
    const auto angles = GetTurnAngles(node);
    auto a = (std::get<1>(angles) - std::get<0>(angles));

    if (a > 180.0)
    {
        a = a - 360.0;
    }

    if (a < -180.0)
    {
        a = a + 360.0;
    }

    if (a < 0)
    {
        a = -a;
    }

    return static_cast<int>(a);
}

const QString GetPicketSignature(const Picket& picket)
{
    static std::map<NodeType, QString> types
    {
        { NodeType::Earthing, "АЗ" },
        { NodeType::HL, "ГЗ" },
        { NodeType::GM, "ГК" },
        { NodeType::Valve, "З" },
        { NodeType::End, "ЗГ" },
        { NodeType::DFC, "ИФС" },
        { NodeType::BValve, "К" },
        { NodeType::CConductor, "КП" },
        { NodeType::CCollector, "КС" },
        { NodeType::CP, "КТ" },
        { NodeType::Material, "М" },
        { NodeType::Protection, "ПЗ" },
        { NodeType::CPS, "СКЗ" },
        { NodeType::Turn, "УП" }
    };

    const auto res = types.find(picket.node->type);
    if (res != types.end())
    {
        if (picket.node->type != NodeType::Turn)
        {
            return res->second;
        }
        else
        {
            const auto angle = GetNodeAngle(*picket.node);

            return angle > 0
                    ? res->second + QString::number(angle) + "\u00B0"
                    : res->second;
        }
    }

    return "";
}

QPainterPath BuildCoordsPath(int x, int y, const std::function<int(int)>& toWidget)
{
    const auto wx = toWidget(x);
    const auto wy = toWidget(y);

    const auto l = toWidget(50);
    const auto w = toWidget(2);
    const auto al = toWidget(50 - 8);
    const auto aw = toWidget(5);

    QPainterPath path;

    path.moveTo(wx - l, wy);
    path.lineTo(wx - al, wy - aw);
    path.lineTo(wx - al, wy - w);
    path.lineTo(wx + al, wy - w);
    path.lineTo(wx + al, wy - aw);

    path.lineTo(wx + l, wy);
    path.lineTo(wx + al, wy + aw);
    path.lineTo(wx + al, wy + w);

    path.lineTo(wx + w, wy + w);
    path.lineTo(wx + w, wy + al);
    path.lineTo(wx + aw, wy + al);

    path.lineTo(wx, wy + l);
    path.lineTo(wx - aw, wy + al);
    path.lineTo(wx - w, wy + al);

    path.lineTo(wx - w, wy + w);
    path.lineTo(wx - al, wy + w);
    path.lineTo(wx - al, wy + aw);

    path.closeSubpath();
    return path;
}

QPainterPath BuildTurnPath(int x, int y, int a, const std::function<int(int)>& toWidget)
{
    const auto l = toWidget(40);
    const auto w = toWidget(2);
    const auto al = toWidget(40 - 8);
    const auto aw = toWidget(5);

    QPainterPath path;

    path.moveTo(0, w);
    path.lineTo(al, w);
    path.lineTo(al, aw);
    path.lineTo(l, 0);
    path.lineTo(al, -aw);
    path.lineTo(al, -w);
    path.lineTo(0, -w);

    path.closeSubpath();

    QTransform t;
    t.translate(toWidget(x), toWidget(y));
    t.rotate(a);

    return t.map(path);
}

int GetSchemaAngle(double realAngle)
{
    if (realAngle > 180.0)
    {
        realAngle -= 360.0;
    }

    if (realAngle < 0.5 && realAngle > -0.5)
    {
        return 0;
    }

    if (realAngle < 90.5 && realAngle > 89.5)
    {
        return 90;
    }

    if (realAngle < -179.5 || realAngle > 179.5)
    {
        return 180;
    }

    if (realAngle < -89.5 && realAngle > -90.5)
    {
        return -90;
    }

    if (realAngle < 90 && realAngle > 0)
    {
        return 45;
    }

    if (realAngle > 90)
    {
        return 135;
    }

    if (realAngle > -90 && realAngle < 0)
    {
        return -45;
    }

    if (realAngle > -90)
    {
        return -135;
    }

    return 0;
}

QPainterPath BuildAnyTurnAnglePath(int a1, int a2, int da, const std::function<int(int)>& toWidget)
{
    const auto l = toWidget(26);
    const auto w = toWidget(1);
    const auto al = toWidget(8);
    const auto aw = toWidget(3);

    QPainterPath path;

    const auto d1 = l + w + l + w;
    const auto d2 = d1 - w*4;

    path.arcMoveTo(-l, -l, d1, d1, a1);
    path.arcTo(-l, -l, d1, d1, a1, da);

    const auto sa1 = std::sin(-a1 * M_PI / 180);
    const auto ca1 = std::cos(-a1 * M_PI / 180);
    const auto sa2 = std::sin(-a2 * M_PI / 180);
    const auto ca2 = std::cos(-a2 * M_PI / 180);
    path.lineTo((l-w*2) * ca2, (l-w*2) * sa2);
    path.arcTo(-l+w, -l+w, d2, d2, a2, -da);

    path.closeSubpath();

    QPainterPath arrow;
    arrow.lineTo(al, aw);
    arrow.lineTo(al, -aw);
    arrow.closeSubpath();

    QTransform ta1;
    ta1.translate((l-w) * ca1, (l-w) * sa1);
    ta1.rotate(-a1 - 90);
    path.addPath(ta1.map(arrow));

    QTransform ta2;
    ta2.translate((l-w) * ca2, (l-w) * sa2);
    ta2.rotate(-a2 + 90);
    path.addPath(ta2.map(arrow));

    return path;
}

QPainterPath BuildNormalTurnAnglePath(int a1, int a2, const std::function<int(int)>& toWidget)
{
    const auto d = toWidget(15);

    QPainterPath path;

    const auto dx = (a1 == 0 || a2 == 0) ? d : -d;
    const auto dy = (a1 == 90 || a2 == 90) ? -d : d;
    const auto dsx = dx*0.93;
    const auto dsy = dy*0.93;

    path.moveTo(dx, 0);
    path.lineTo(dx, dy);
    path.lineTo(0, dy);
    path.lineTo(0, dsy);
    path.lineTo(dsx, dsy);
    path.lineTo(dsx, 0);

    return path;
}

QPainterPath BuildTurnAnglePath(int x, int y, const std::tuple<int, int>& angles, const std::function<int(int)>& toWidget)
{
    auto a1 = -std::get<0>(angles);
    auto a2 = -std::get<1>(angles);
    if (a2 < a1)
    {
        a2 = a2 + 360;
    }
    auto da = a2 - a1;

    if (std::abs(da) > 180)
    {
        a1 = -std::get<1>(angles);
        a2 = -std::get<0>(angles);
        if (a2 < a1)
        {
            a2 = a2 + 360;
        }
        da = a2 - a1;
    }

    if (da == 0 || da == 180 || da == -180)
    {
        return {};
    }


    QTransform t;
    t.translate(toWidget(x), toWidget(y));

    if (da == 90 || da == -90)
    {
        return t.map(BuildNormalTurnAnglePath(a1, a2, toWidget));
    }
    else
    {
        return t.map(BuildAnyTurnAnglePath(a1, a2, da, toWidget));
    }
}

void PicketView::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen{ QColor { 0, 0, 0 } });

    const auto node = picket->node;
    const auto section = node->section;
    const auto pipeline = section->pipeline;

    const auto s = height() / 200.0;
    const auto toWidget = [s](int picket) { return static_cast<int>(picket * s); };

    QFont font30{ "Arial" };
    font30.setPixelSize(toWidget(30));

    if (section->material == PipeMaterial::Pe)
    {
        painter.setBrush(QBrush { QColor { 255, 245, 0 } });
    }
    else
    {
        if (status == Status::Active)
        {
            if (pipeline->pressure == PressureType::G4)
            {
                painter.setPen(QPen{ QColor { 0xD0, 0x30, 0x2B } });
                painter.setBrush(QBrush { QColor { 0xD0, 0x30, 0x2B } });
                painter.drawRect(0, 0, width()-1, height()-1);

                painter.setPen(QPen{ QColor { 0, 126, 82 } });
                painter.setBrush(QBrush { QColor { 0, 126, 82 } });
                painter.drawRect(toWidget(5), toWidget(5), toWidget(130), toWidget(190));

                painter.setPen(QPen{ QColor { 0, 0, 0 } });
            }
            else
            {
                painter.setBrush(QBrush { QColor { 0, 126, 82 } });
                painter.drawRect(0, 0, width()-1, height()-1);
            }
        }
        else
        {
            painter.setBrush(QBrush { QColor { 200, 200, 200 } });
            painter.drawRect(0, 0, width()-1, height()-1);
        }

        painter.setFont(font30);

        QRect line1Rect { toWidget(10), toWidget(10), toWidget(120), toWidget(30) };
        painter.drawText(line1Rect, Qt::AlignLeft | Qt::AlignBottom, GetPressureSteelTitle(pipeline->pressure), nullptr);
        painter.drawText(line1Rect, Qt::AlignRight | Qt::AlignBottom, QString::number(section->diameter), nullptr);

        QRect line2Rect { toWidget(10), toWidget(50), toWidget(120), toWidget(30) };
        painter.drawText(line2Rect, Qt::AlignHCenter | Qt::AlignBottom, GetPicketSignature(*picket), nullptr);

        painter.setBrush(QBrush { QColor { 0, 0, 0 } });
        painter.drawPath(BuildCoordsPath(70, 110, toWidget));

        if (node->type == NodeType::Turn)
        {
            const auto angles = GetTurnAngles(*node);

            const auto a1 = std::get<0>(angles);
            const auto sa1 = GetSchemaAngle(a1);
            if (sa1 != 0 && sa1 != 90 && sa1 != 180)
            {
                painter.drawPath(BuildTurnPath(70, 110, sa1, toWidget));
            }

            const auto a2 = std::get<1>(angles);
            const auto sa2 = GetSchemaAngle(a2);
            if (sa2 != 0 && sa2 != 90 && sa2 != 180)
            {
                painter.drawPath(BuildTurnPath(70, 110, sa2, toWidget));
            }

            painter.drawPath(BuildTurnAnglePath(70, 110, std::make_tuple(sa1, sa2), toWidget));
        }

        if (status == Status::Succeed)
        {
            painter.setPen(QPen{ QColor { 30, 150, 30 }, 10.0 });
            painter.drawLine(toWidget(80), toWidget(30), toWidget(100), toWidget(50));
            painter.drawLine(toWidget(100), toWidget(50), toWidget(120), toWidget(10));
        }
        else if (status == Status::Failed)
        {
            painter.setPen(QPen{ QColor { 150, 30, 30 }, 10.0 });
            painter.drawLine(toWidget(80), toWidget(10), toWidget(120), toWidget(50));
            painter.drawLine(toWidget(80), toWidget(50), toWidget(120), toWidget(10));
        }
    }
}

void PicketView::makeActive()
{
    status = Status::Active;
    update();
}

void PicketView::makeSuccess()
{
    status = Status::Succeed;
    update();
}

void PicketView::makeFailed()
{
    status = Status::Failed;
    update();
}
