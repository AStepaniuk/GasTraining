#include "turngeometry.h"

#include <tuple>
#include <cmath>
#include <algorithm>

namespace TurnGeometryImpl
{
    std::tuple<double, double> GetPipesAngles(const Node& node)
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

    double GetNodeAngle(const std::tuple<double, double>& pipesAngles)
    {
        auto a = (std::get<1>(pipesAngles) - std::get<0>(pipesAngles));

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

        return a;
    }

    int GetPicketAngle(double realAngle)
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

        if (realAngle < -90)
        {
            return -135;
        }

        return 0;
    }
}

using namespace TurnGeometryImpl;

TurnGeometry::Data TurnGeometry::Calculate(const Node &node)
{
    Data result;

    const auto pipesAngles = GetPipesAngles(node);
    result.angle1 = std::get<0>(pipesAngles);
    result.angle2 = std::get<1>(pipesAngles);

    const auto turnAngle = GetNodeAngle(pipesAngles);
    result.turnAngle = turnAngle;

    result.picketAngle1 = GetPicketAngle(result.angle1);
    result.picketAngle2 = GetPicketAngle(result.angle2);

    result.picketTurnAngle = static_cast<int>(turnAngle);

    return result;
}
