#include "SphereOrthogonalProjectionScene.h"

#include <QGraphicsSceneMouseEvent>
#include <QtDebug>
#include <iostream>

const RotationTriple<double>& SphereOrthogonalProjectionScene::getPerspectiveTriple() const {
    return perspectiveTriple;
}

const Vector3d<double> &SphereOrthogonalProjectionScene::i() const {
    return perspectiveTriple.getPointer();
}

const Vector3d<double> &SphereOrthogonalProjectionScene::j() const {
    return perspectiveTriple.getTangent();
}

const Vector3d<double> &SphereOrthogonalProjectionScene::k() const {
    return perspectiveTriple.getGuide();
}

double SphereOrthogonalProjectionScene::getProjectionCircleRadius() const {
    return projectionCircleRadius;
}

void SphereOrthogonalProjectionScene::setProjectionCircleRadius(double newProjectionCircleRadius) {
    projectionCircleRadius = newProjectionCircleRadius;
    projectionCircle->setRect(-projectionCircleRadius, -projectionCircleRadius,
                              2 * projectionCircleRadius, 2 * projectionCircleRadius);
    updateAllPointDisplays();

    emit projectionCircleRadiusChanged();
}

SphereGravityModel *SphereOrthogonalProjectionScene::getGravityModel() const {
    return gravityModel;
}

QGraphicsEllipseItem *SphereOrthogonalProjectionScene::getProjectionCircle() const {
    return projectionCircle;
}

SphereOrthogonalProjectionScene::SphereOrthogonalProjectionScene(SphereGravityModel* _gravityModel,
                                                                 QObject* parent)
    : QGraphicsScene(parent),
      gravityModel(_gravityModel),
      perspectiveTriple(RotationTriple(Vector3d<double>(1, 0, 0),
                                       Vector3d<double>(0, 1, 0))) {
    backgroundBrush = QBrush(Qt::black);
    setBackgroundBrush(backgroundBrush);

    projectionCircle = addEllipse(0, 0, 0, 0, QPen(Qt::white));
    setProjectionCircleRadius(200);

    oxLineItem = addLine(0, 0, 0, 0);
    oyLineItem = addLine(0, 0, 0, 0);
    ozLineItem = addLine(0, 0, 0, 0);
    updateAxisLines();
}

void SphereOrthogonalProjectionScene::addMaterialPoint(SphereGravityModel::Iterator pointIterator) {
    auto pos = pointIterator->getPosition();
    auto [x, y] = projectOnXOY(pos);
    auto ellipseItem = addEllipse(x, y, 5, 5, QPen(Qt::yellow), QBrush(Qt::yellow));
    pointContainerList.push_back({ellipseItem, pointIterator});
    updatePointDisplay(&pointContainerList.back());
}

std::pair<double, double> SphereOrthogonalProjectionScene::projectOnXOY(Vector3d<double> vector) {
    Vector3d<double> i = perspectiveTriple.getPointer();
    Vector3d<double> j = perspectiveTriple.getTangent();
    Vector3d<double> k = perspectiveTriple.getGuide();

    double x;
    double y;

    x =   vector.x * (j.y * k.z - k.y * j.z)
        - vector.y * (j.x * k.z - k.x * j.z)
        + vector.z * (j.x * k.y - k.x * j.y);

    y =  -vector.x * (i.y * k.z - k.y * i.z)
        + vector.y * (i.x * k.z - k.x * i.z)
        - vector.z * (i.x * k.y - k.x * i.y);

    return { x, y };
}

double SphereOrthogonalProjectionScene::projectOnOZ(Vector3d<double> vector) {
    Vector3d<double> i = perspectiveTriple.getPointer();
    Vector3d<double> j = perspectiveTriple.getTangent();

    return   vector.x * (i.y * j.z - j.y * i.z)
           - vector.y * (i.x * j.z - j.x * i.z)
           + vector.z * (i.x * j.y - j.x * i.y);
}

void SphereOrthogonalProjectionScene::rotatePerspectiveOX(double angle) {
    auto rotatedPerspectiveTriple = perspectiveTriple.previousCyclicShift().rotatedTriple(angle);
    perspectiveTriple = rotatedPerspectiveTriple.nextCyclicShift();
}

void SphereOrthogonalProjectionScene::rotatePerspectiveOY(double angle) {
    auto rotatedPerspectiveTriple = perspectiveTriple.nextCyclicShift().rotatedTriple(angle);
    perspectiveTriple = rotatedPerspectiveTriple.previousCyclicShift();
}

void SphereOrthogonalProjectionScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mouseMoveEvent(event);

    Qt::MouseButtons buttons = event->buttons();

    double curX = event->scenePos().x();
    double curY = event->scenePos().y();

    if (buttons & Qt::MouseButton::RightButton) {
        qInfo() << "RightButton pressed";

        double deltaX = (double)(event->scenePos().x() - event->lastScenePos().x());
        double deltaY = (double)(event->scenePos().y() - event->lastScenePos().y());

        rotatePerspectiveOY(deltaX * 0.01);
        rotatePerspectiveOX(deltaY * 0.01);

        qDebug() << "\nperspectiveTriple after rotations: ";
        perspectiveTriple.debug();
        qDebug() << curX << " " << curY;

        updateAxisLines();

        emit perspectiveChanged();

        updateAllPointDisplays();

        setBackgroundBrush(backgroundBrush);
    }
}

SphereOrthogonalProjectionScene::~SphereOrthogonalProjectionScene() {}

void SphereOrthogonalProjectionScene::updatePointDisplay(PointContainer* pointContainer) {
    auto [x, y] = projectOnXOY(pointContainer->pointIterator->getPosition());
    auto z = projectOnOZ(pointContainer->pointIterator->getPosition());

    if (z >= 0) {
        pointContainer->ellipseItem->setBrush(QBrush(Qt::yellow));
        pointContainer->ellipseItem->setRect(x * projectionCircleRadius, -y * projectionCircleRadius, 4 + 2 * z, 4 + 2 * z);
    } else {
        pointContainer->ellipseItem->setBrush(QBrush(Qt::yellow, Qt::BrushStyle::NoBrush));
        pointContainer->ellipseItem->setRect(x * projectionCircleRadius, -y * projectionCircleRadius, 4 + 2 * z, 4 + 2 * z);
    }
}

void SphereOrthogonalProjectionScene::updateAllPointDisplays() {
    for(auto& pointContainer : pointContainerList) {
        /*
        auto [x, y] = projectOnXOY(pointContainer.pointIterator->getPosition());
        auto z = projectOnOZ(pointContainer.pointIterator->getPosition());

        if (z >= 0) {
            pointContainer.ellipseItem->setBrush(QBrush(Qt::yellow));
            pointContainer.ellipseItem->setRect(x * projectionCircleRadius, -y * projectionCircleRadius, 4 + 2 * z, 4 + 2 * z);
        } else {
            pointContainer.ellipseItem->setBrush(QBrush(Qt::yellow, Qt::BrushStyle::NoBrush));
            pointContainer.ellipseItem->setRect(x * projectionCircleRadius, -y * projectionCircleRadius, 4 + 2 * z, 4 + 2 * z);
        }
        */
        updatePointDisplay(&pointContainer);
    }

    setBackgroundBrush(backgroundBrush);
}

void SphereOrthogonalProjectionScene::updateAxisLines() {
    auto [x1, y1] = projectOnXOY({1, 0, 0});
    oxLineItem->setLine(0, 0, x1 * projectionCircleRadius, -y1 * projectionCircleRadius);
    if (projectOnOZ({1, 0, 0}) >= 0) {
        oxLineItem->setPen(QPen(Qt::red, 2));
    } else {
        oxLineItem->setPen(QPen(Qt::red, 1, Qt::PenStyle::DotLine));
    }

    auto [x2, y2] = projectOnXOY({0, 1, 0});
    oyLineItem->setLine(0, 0, x2 * projectionCircleRadius, -y2 * projectionCircleRadius);
    if (projectOnOZ({0, 1, 0}) >= 0) {
        oyLineItem->setPen(QPen(Qt::green, 2));
    } else {
        oyLineItem->setPen(QPen(Qt::green, 1, Qt::PenStyle::DotLine));
    }

    auto [x3, y3] = projectOnXOY({0, 0, 1});
    ozLineItem->setLine(0, 0, x3 * projectionCircleRadius, -y3 * projectionCircleRadius);
    if (projectOnOZ({0, 0, 1}) >= 0) {
        ozLineItem->setPen(QPen(Qt::blue, 2));
    } else {
        ozLineItem->setPen(QPen(Qt::blue, 1, Qt::PenStyle::DotLine));
    }
}
