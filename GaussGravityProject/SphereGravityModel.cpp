#include "SphereGravityModel.h"

const SphereGravityModel::ConcentrationType SphereGravityModel::STANDART_C_FUNC =
        [](const SphereMaterialPoint& a, const SphereMaterialPoint& b)
{
    double angle = a.getPosition().angle(b.getPosition());
    return 1 / angle / angle - 1 / (6.28 - angle) / (6.28 - angle);
};

SphereGravityModel::SphereGravityModel(const ConcentrationType& _concentration)
    : concentration(_concentration) {}

void SphereGravityModel::step(double time) {
    Vector3d<double> accelerations[pointList.size()];  // tangent accelerations

    int i = 0;
    for (auto first = pointList.begin(); first != pointList.end(); ++first, ++i) {
        Vector3d<double> firstPosition = first->getPosition();
        accelerations[i] = {0, 0, 0};

        int j = 0;
        for (auto second = pointList.begin(); second != first; ++second, ++j) {
            Vector3d<double> secondPosition = second->getPosition();  // Maybe requires rework

            double angle = firstPosition.angle(secondPosition);

            if (abs(angle) < 1e-1) continue;  // Damn

            double rho = concentration(*first, *second);

            Vector3d<double> firstProjection = firstPosition
                                               - firstPosition.component(secondPosition);
            Vector3d<double> secondProjection = secondPosition
                                                - secondPosition.component(firstPosition);

            accelerations[i] += secondProjection.normalized() * second->getMass() * rho;
            accelerations[j] += firstProjection.normalized() * first->getMass() * rho;
        }
    }

    i = 0;
    for (auto first = pointList.begin(); first != pointList.end(); ++first, ++i) {
        Vector3d<double> newVelocity = first->getVelocityDirection() * first->getAngularVelocity()
                                       + accelerations[i] * time;
        first->changeMovementDirection(newVelocity);
        first->setAngularVelocity(newVelocity.length());
        first->move(time);
        Vector3d<double> pos = first->getPosition();
        qDebug() << "point: " << pos.x << ' ' << pos.y << ' ' << pos.z;
    }
    qDebug() << '\n';
}

SphereGravityModel::Iterator SphereGravityModel::addPoint(const Vector3d<double>& pointPosition, double mass) {
    pointList.push_back(SphereMaterialPoint(pointPosition, mass));
    return --pointList.end();
}

SphereGravityModel::Iterator SphereGravityModel::addPoint(const SphereMaterialPoint& point) {
    pointList.push_back(point);
    return --pointList.end();
}

const std::list<SphereMaterialPoint> &SphereGravityModel::getPointList() const {
    return pointList;
}

SphereGravityModel::~SphereGravityModel() {}
