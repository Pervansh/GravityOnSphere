#pragma once

#include "RotationTriple.h"
#include "Vector3d.h"

class SphereMaterialPoint {
private:
    RotationTriple<double> positionVelocityTriple;
    double angularVelocity;
    double mass;

public:
    SphereMaterialPoint(const Vector3d<double>& position, double _mass);

    SphereMaterialPoint(const Vector3d<double>& position,
                        const Vector3d<double>& movementDirection,
                        double _angularVelocity,
                        double _mass);

    void move(double time);

    Vector3d<double> getVelocityDirection() const;
    Vector3d<double> getPosition()          const;
    double           getAngularVelocity()   const { return angularVelocity; }
    double           getMass()              const;

    void setAngularVelocity     (double newAngularVelocity);
    void setMass                (double newMass);
    void changeMovementDirection(const Vector3d<double>& newMovementDirection);
    // void setVelocityDirection(Vector3d<double> newVelocityDirection) ;
    // void setPosition         (Vector3d<double> newPosition)          ;
};
