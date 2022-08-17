#include "SphereOrthogonalProjectionScene.h"

#include <QGraphicsSceneMouseEvent>
#include <QtDebug>
#include <iostream>

const RotationTriple<double>& SphereOrthogonalProjectionScene::getPerspectiveTriple() const {
    return perspectiveTriple;
}

SphereOrthogonalProjectionScene::SphereOrthogonalProjectionScene(SphereGravityModel* _gravityModel,
                                                                 QObject* parent)
    : QGraphicsScene(parent),
      gravityModel(_gravityModel),
      perspectiveTriple(RotationTriple(Vector3d<double>(1, 0, 0),
                                       Vector3d<double>(0, 1, 0))) {
    backgroundBrush = QBrush(Qt::black);
    setBackgroundBrush(backgroundBrush);

    addEllipse(0, 0, 300, 300, QPen(Qt::white));

    auto [x1, y1] = projectOnXOY({1, 0, 0});
    oxLineItem = addLine(150, 150, 150 + x1 * 150, 150 - y1 * 150, QPen(Qt::red));

    auto [x2, y2] = projectOnXOY({0, 1, 0});
    oyLineItem = addLine(150, 150, 150 + x2 * 150, 150 - y2 * 150, QPen(Qt::green));

    auto [x3, y3] = projectOnXOY({0, 0, 1});
    ozLineItem = addLine(150, 150, 150 + x3 * 150, 150 - y3 * 150, QPen(Qt::blue));

    perspectiveTriple.debug();
}

void SphereOrthogonalProjectionScene::addMaterialPoint(SphereGravityModel::Iterator pointIterator) {
    auto pos = pointIterator->getPosition();
    auto [x, y] = projectOnXOY(pos);
    auto ellipseItem = addEllipse(x, y, 5, 5, QPen(Qt::yellow), QBrush(Qt::yellow));
    pointContainerList.push_back({ellipseItem, pointIterator});
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
    qInfo() << "mouseMoveEvent";

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

        auto [x1, y1] = projectOnXOY({1, 0, 0});
        oxLineItem->setLine(150, 150, 150 + x1 * 150, 150 - y1 * 150);
        if (projectOnOZ({1, 0, 0}) >= 0) {
            oxLineItem->setPen(QPen(Qt::red, 2));
        } else {
            oxLineItem->setPen(QPen(Qt::red, 1, Qt::PenStyle::DotLine));
        }

        auto [x2, y2] = projectOnXOY({0, 1, 0});
        oyLineItem->setLine(150, 150, 150 + x2 * 150, 150 - y2 * 150);
        if (projectOnOZ({0, 1, 0}) >= 0) {
            oyLineItem->setPen(QPen(Qt::green, 2));
        } else {
            oyLineItem->setPen(QPen(Qt::green, 1, Qt::PenStyle::DotLine));
        }

        auto [x3, y3] = projectOnXOY({0, 0, 1});
        ozLineItem->setLine(150, 150, 150 + x3 * 150, 150 - y3 * 150);
        if (projectOnOZ({0, 0, 1}) >= 0) {
            ozLineItem->setPen(QPen(Qt::blue, 2));
        } else {
            ozLineItem->setPen(QPen(Qt::blue, 1, Qt::PenStyle::DotLine));
        }

        emit perspectiveChange();

        updatePointDisplays();

        setBackgroundBrush(backgroundBrush);
    }
}

SphereOrthogonalProjectionScene::~SphereOrthogonalProjectionScene() {}

void SphereOrthogonalProjectionScene::updatePointDisplays() {
    for(auto& pointContainer : pointContainerList) {
        auto [x, y] = projectOnXOY(pointContainer.pointIterator->getPosition());
        auto z = projectOnOZ(pointContainer.pointIterator->getPosition());

        if (z >= 0) {
            pointContainer.ellipseItem->setBrush(QBrush(Qt::yellow));
            pointContainer.ellipseItem->setRect(150 + x * 150, 150 - y * 150, 4 + 2 * z, 4 + 2 * z);
        } else {
            pointContainer.ellipseItem->setBrush(QBrush(Qt::yellow, Qt::BrushStyle::NoBrush));
            pointContainer.ellipseItem->setRect(150 + x * 150, 150 - y * 150, 4 + 2 * z, 4 + 2 * z);
        }
    }

    setBackgroundBrush(backgroundBrush);
}
