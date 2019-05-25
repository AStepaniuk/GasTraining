#ifndef TURNGEOMETRY_H
#define TURNGEOMETRY_H

#include "Data/model.h"

class TurnGeometry
{
public:
    struct Data
    {
        double angle1;
        double angle2;

        double turnAngle;

        int picketAngle2;
        int picketAngle1;

        int picketTurnAngle;
    };

    static Data Calculate(const Node& node);
};

#endif // TURNGEOMETRY_H
