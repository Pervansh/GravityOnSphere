#include "SphereOrthogonalProjectionArcDrawer.h"
#include <cmath>
#include <qmath.h>
#include "RotationTriple.h"
#include "Vector2d.h"

SphereOrthogonalProjectionArcDrawer::SphereOrthogonalProjectionArcDrawer(SphereOrthogonalProjectionScene* _scene)
    : QObject(_scene), scene(_scene) {
    ellipseLine = scene->addLine(150, 150, 300, 150, QPen(Qt::white));
    ellipseLine->setTransformOriginPoint(150, 150);
}

SphereOrthogonalProjectionArcDrawer::Arc*
SphereOrthogonalProjectionArcDrawer::addArc(const Vector3d<double> &startPoint,
                                            const Vector3d<double> &endPoint,
                                            const QPen &pen,
                                            bool isEllipse)
{
    Arc* arc;

    if (vacantArcList.empty()) {
        arc = new Arc(scene->addEllipse(0, 0, 0, 0));
        arc->item->setTransformOriginPoint(150, 150);
        allArcList.push_back(arc);

        arc->isEllipse = isEllipse;
        arc->startPlanePoint = startPoint;
        arc->endPlanePoint = endPoint;
        arc->pen = pen;

        drawProjection(arc);
    } else {
        arc = vacantArcList.back();
        arc->item->setVisible(true);
        vacantArcList.pop_back();
    }

    return arc;
}

SphereOrthogonalProjectionArcDrawer::Arc
*SphereOrthogonalProjectionArcDrawer::addEllipse(const Vector3d<double> &planePoint1,
                                                 const Vector3d<double> &planePoint2,
                                                 const QPen &pen)
{
    return addArc(planePoint1, planePoint2, pen, true);
}

void SphereOrthogonalProjectionArcDrawer::removeArc(Arc* arc) {
    arc->item->setVisible(false);
    vacantArcList.push_back(arc);
}

SphereOrthogonalProjectionArcDrawer::~SphereOrthogonalProjectionArcDrawer() {
    for (auto arc : allArcList) {
        delete arc;
    }
}

void SphereOrthogonalProjectionArcDrawer::drawProjection(Arc *arc) {
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

    arc->item->setPen(arc->pen);
    arc->item->setRect(150 - a, 150 - b, 2 * a, 2 * b); //the projection ellipse defaultly isn't rotated and lies along OX axis

    if (arc->isEllipse) {
        arc->item->setSpanAngle(5760); // default span angle value for ellipse

        ellipseLine->setRotation(qRadiansToDegrees(intersectionGuideAngle));
    } else {
        auto [startX, startY] = scene->projectOnXOY(arc->startPlanePoint);
        Bismath::Vector2d startProjection = {startX, startY};
        auto [endX, endY] = scene->projectOnXOY(arc->endPlanePoint);
        Bismath::Vector2d endProjection = {endX, endY};
        // ! Нет проблем

        // Инфа по поводу бага с отображением дуги:
        // Дуга в половине случаев отображается зеркально от нужного положения относительно главной оси, если есть sign
        // Дуга закрепляется относительно правильно, если посмотреть на плоскость с другой стороны

        // Ellipse is just a sqiushed circle. Function requires angle in not sqiushed circle
        arc->item->setStartAngle(16 * sign(cross(intersectionGuideProjection, startProjection))
                                    * qRadiansToDegrees(arc->startPlanePoint
                                                        .angle(intersectionGuide)));
        arc->item->setSpanAngle(16 * sign(cross(startProjection, endProjection))
                                   * qRadiansToDegrees(arc->startPlanePoint
                                                       .angle(arc->endPlanePoint)));
    }

    arc->item->setRotation(qRadiansToDegrees(intersectionGuideAngle));  // rotating the projection ellipse

}

void SphereOrthogonalProjectionArcDrawer::updateProjections() {
    for (auto arc : allArcList) {
        drawProjection(arc);
    }
}
