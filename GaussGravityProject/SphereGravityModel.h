#pragma once

#include <list>
#include <functional>

#include "SphereMaterialPoint.h"
#include "Vector3d.h"

class SphereGravityModel {
public:
    using Iterator = std::list<SphereMaterialPoint>::iterator;
    using ConcentrationType = std::function<double(const SphereMaterialPoint&,
                                                   const SphereMaterialPoint&)>;
private:
    std::list<SphereMaterialPoint> pointList;
    ConcentrationType concentration;

public:

    static const ConcentrationType STANDART_C_FUNC;

    SphereGravityModel(const ConcentrationType&);

    /** time usually in seconds */
    void step(double time);

    Iterator addPoint   (const Vector3d<double>& pointPosition, double mass);
    Iterator addPoint   (const SphereMaterialPoint&);
    void     deletePoint(Iterator iterator);

    const std::list<SphereMaterialPoint>& getPointList() const;
};
