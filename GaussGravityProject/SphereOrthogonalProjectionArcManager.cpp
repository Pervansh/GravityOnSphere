#include "SphereOrthogonalProjectionArcManager.h"
#include <cmath>
#include <qmath.h>
#include "RotationTriple.h"
#include "Vector2d.h"

SphereOrthogonalProjectionArcManager::SphereOrthogonalProjectionArcManager(SphereOrthogonalProjectionScene* _scene)
    : QObject(_scene), scene(_scene) {
    ellipseLine = scene->addLine(150, 150, 300, 150, QPen(Qt::white));
    ellipseLine->setTransformOriginPoint(150, 150);
}

SphereOrthogonalProjectionArcManager::Arc*
SphereOrthogonalProjectionArcManager::addArc(const Vector3d<double> &startPoint,
                                            const Vector3d<double> &endPoint,
                                            const QPen &foregroundPen,
                                            bool isClosed)
{
    QPen backgroundPen = foregroundPen;
    backgroundPen.setStyle(Qt::PenStyle::DotLine);
    return addArc(startPoint, endPoint, foregroundPen, backgroundPen, isClosed);
}

SphereOrthogonalProjectionArcManager::Arc*
SphereOrthogonalProjectionArcManager::addArc(const Vector3d<double> &startPoint,
                                             const Vector3d<double> &endPoint,
                                             const QPen &foregroundPen,
                                             const QPen &backgroundPen,
                                             bool isClosed)
{
    Arc* arc;

    if (vacantArcList.empty()) {
        arc = new Arc(scene->addPath(QPainterPath()), scene->addPath(QPainterPath()));
        arc->foregroundItem->setTransformOriginPoint(150, 150);
        arc->backgroundItem->setTransformOriginPoint(150, 150);
        allArcList.push_back(arc);
    } else {
        arc = vacantArcList.back();
        arc->foregroundItem->setVisible(true);
        arc->backgroundItem->setVisible(true);
        vacantArcList.pop_back();
    }

    arc->isClosed = isClosed;
    arc->startPlanePoint = startPoint;
    arc->endPlanePoint = endPoint;
    arc->foregroundPen = foregroundPen;
    arc->backgroundPen = backgroundPen;

    drawProjection(arc);

    return arc;
}

SphereOrthogonalProjectionArcManager::Arc*
SphereOrthogonalProjectionArcManager::addClosedArc(const Vector3d<double> &planePoint1,
                                                   const Vector3d<double> &planePoint2,
                                                   const QPen &foregroundPen)
{
    return addArc(planePoint1, planePoint2, foregroundPen, true);
}

SphereOrthogonalProjectionArcManager::Arc*
SphereOrthogonalProjectionArcManager::addClosedArc(const Vector3d<double> &planePoint1,
                                                   const Vector3d<double> &planePoint2,
                                                   const QPen &foregroundPen,
                                                   const QPen &backgroundPen)
{
    return addArc(planePoint1, planePoint2, foregroundPen, backgroundPen, true);
}

void SphereOrthogonalProjectionArcManager::removeArc(Arc* arc) {
    arc->foregroundItem->setVisible(false);
    arc->backgroundItem->setVisible(false);
    vacantArcList.push_back(arc);
}

SphereOrthogonalProjectionArcManager::~SphereOrthogonalProjectionArcManager() {
    for (auto arc : allArcList) {
        delete arc;
    }
}

void SphereOrthogonalProjectionArcManager::drawProjection(Arc *arc) {
    using Bismath::sign;

    Vector3d<double> arcPlaneNormal = cross(arc->startPlanePoint, arc->endPlanePoint);
    Vector3d<double> perspectivePlaneNormal = scene->getPerspectiveTriple().getGuide();
    Vector3d<double> intersectionGuide = cross(arcPlaneNormal, perspectivePlaneNormal);  // Vector of major axis of arc's projection ellipse

    Vector3d<double> intersectionNormal = arc->startPlanePoint  // can be null and cause error further
                                          - arc->startPlanePoint.component(intersectionGuide);

    // a - major semiaxis of the projection ellipse, b - minor semiaxis that defines by angle between planes
    double a = 150;
    double b = a * std::abs(std::sin(intersectionNormal.angle(perspectivePlaneNormal)));  // potential error, not critical

    auto [projectionX, projectionY] = scene->projectOnXOY(intersectionGuide);
    Bismath::Vector2d intersectionGuideProjection = {projectionX, projectionY};

    double intersectionGuideAngle = intersectionGuideProjection.angle({1, 0});  // angle between intersection guide and OX

    // arc->item->setRect(150 - a, 150 - b, 2 * a, 2 * b); //the projection ellipse defaultly isn't rotated and lies along OX axis

    auto [startX, startY] = scene->projectOnXOY(arc->startPlanePoint);
    Bismath::Vector2d startProjection = {startX, startY};
    auto [endX, endY] = scene->projectOnXOY(arc->endPlanePoint);
    Bismath::Vector2d endProjection = {endX, endY};

    QPainterPath foregroundPath;
    QPainterPath backgroundPath;

    if (arc->isClosed) {
        double sgn = sign(cross(startProjection, endProjection));

        foregroundPath.moveTo(300, 150);
        arc->foregroundItem->setPen(arc->foregroundPen);
        foregroundPath.arcTo(150 - a, 150 - b, 2 * a, 2 * b, 0, -180 * sgn);

        backgroundPath.moveTo(300, 150);
        arc->backgroundItem->setPen(arc->backgroundPen);
        backgroundPath.arcTo(150 - a, 150 - b, 2 * a, 2 * b, 0, 180 * sgn);

        ellipseLine->setRotation(qRadiansToDegrees(intersectionGuideAngle));
    } else {
        // Инфа по поводу бага с отображением дуги:
        // Дуга в половине случаев отображается зеркально от нужного положения относительно главной оси, если есть sign
        // Дуга закрепляется относительно правильно, если посмотреть на плоскость с другой стороны

        // Ellipse is just a sqiushed circle. Function requires angle in not sqiushed circle
        double startAngle = sign(cross(intersectionGuideProjection, startProjection))
                            * qRadiansToDegrees(arc->startPlanePoint
                                                .angle(intersectionGuide));
        double sweepLength = sign(cross(startProjection, endProjection))
                             * qRadiansToDegrees(arc->startPlanePoint
                                                 .angle(arc->endPlanePoint));

        foregroundPath.arcMoveTo(150 - a, 150 - b, 2 * a, 2 * b, startAngle);
        arc->foregroundItem->setPen(arc->foregroundPen);
        foregroundPath.arcTo(150 - a, 150 - b, 2 * a, 2 * b, startAngle, sweepLength);
    }

    arc->foregroundItem->setPath(foregroundPath);
    arc->foregroundItem->setRotation(qRadiansToDegrees(intersectionGuideAngle));  // rotating the projection ellipse
    arc->backgroundItem->setPath(backgroundPath);
    arc->backgroundItem->setRotation(qRadiansToDegrees(intersectionGuideAngle));  // rotating the projection ellipse

}

void SphereOrthogonalProjectionArcManager::updateProjections() {
    for (auto arc : allArcList) {
        drawProjection(arc);
    }
}

const QPen& SphereOrthogonalProjectionArcManager::Arc::getForegroundPen() const
{
    return foregroundPen;
}

void SphereOrthogonalProjectionArcManager::Arc::setForegroundPen(const QPen &newForegroundPen)
{
    foregroundPen = newForegroundPen;
    foregroundItem->setPen(newForegroundPen);
}

const QPen& SphereOrthogonalProjectionArcManager::Arc::getBackgroundPen() const
{
    return backgroundPen;
}

void SphereOrthogonalProjectionArcManager::Arc::setBackgroundPen(const QPen &newBackgroundPen)
{
    backgroundPen = newBackgroundPen;
    backgroundItem->setPen(newBackgroundPen);
}

const Vector3d<double>&
SphereOrthogonalProjectionArcManager::Arc::getStartPlanePoint() const
{
    return startPlanePoint;
}

void SphereOrthogonalProjectionArcManager::Arc::setStartPlanePoint(const Vector3d<double> &newStartPlanePoint)
{
    startPlanePoint = newStartPlanePoint;
}

const Vector3d<double>&
SphereOrthogonalProjectionArcManager::Arc::getEndPlanePoint() const
{
    return endPlanePoint;
}

void SphereOrthogonalProjectionArcManager::Arc::setEndPlanePoint(const Vector3d<double> &newEndPlanePoint)
{
    endPlanePoint = newEndPlanePoint;
}
