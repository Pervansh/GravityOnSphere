#include "SphereMaterialPoint.h"

SphereMaterialPoint::SphereMaterialPoint(const Vector3d<double>& position, double _mass)
    : positionVelocityTriple({1, 0, 0}, {0, 1, 0}), angularVelocity(0), mass(_mass){

    if (position.x != 0) {
        positionVelocityTriple = RotationTriple<double>(position, {0, 1, 1});
    } else if (position.y != 0) {
        positionVelocityTriple = RotationTriple<double>(position, {1, 0, 1});
    } else if (position.z != 0) {
        positionVelocityTriple = RotationTriple<double>(position, {1, 1, 0});
    } else {
        qFatal("The null vector cannot be passed as position parametr in SphereMaterialPoint constructor.");
    }
}


// Can have issues when position = movementDirection
SphereMaterialPoint::SphereMaterialPoint(const Vector3d<double>& position,
                                         const Vector3d<double>& movementDirection,
                                         double _angularVelocity,
                                         double _mass)
    : positionVelocityTriple(position, movementDirection),
      angularVelocity(_angularVelocity),
      mass(_mass) {}


void SphereMaterialPoint::move(double time) {
    positionVelocityTriple.rotateTriple(angularVelocity * time);
}

Vector3d<double> SphereMaterialPoint::getPosition() const {
    return positionVelocityTriple.getPointer();
}

Vector3d<double> SphereMaterialPoint::getVelocityDirection() const {
    return positionVelocityTriple.getTangent();
}

double SphereMaterialPoint::getMass() const {
    return mass;
}

void SphereMaterialPoint::setMass(double newMass) {
    mass = newMass;
}

void SphereMaterialPoint::setAngularVelocity(double newAngularVelocity) {
    angularVelocity = newAngularVelocity;
}

void SphereMaterialPoint::changeMovementDirection(const Vector3d<double>& newMovementDirection) {
    if (positionVelocityTriple.getPointer().cross(newMovementDirection).length() == 0) {
        qFatal("newMovementDirection should not be proportional to position of the point.");
    }

    positionVelocityTriple = RotationTriple<double>(positionVelocityTriple.getPointer(),
                                                    newMovementDirection);
}
