#pragma once

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QColor>
#include <utility>
#include <list>
#include "RotationTriple.h"
#include "SphereGravityModel.h"
#include "SphereMaterialPoint.h"

class SphereOrthogonalProjectionScene : public QGraphicsScene {
    Q_OBJECT

public:
    class PointContainer {
        friend SphereOrthogonalProjectionScene;

    private:
        QGraphicsEllipseItem* ellipseItem;
        SphereGravityModel::Iterator pointIterator;

        PointContainer(QGraphicsEllipseItem* _ellipseItem,
                       SphereGravityModel::Iterator _pointIterator)
            : ellipseItem(_ellipseItem), pointIterator(_pointIterator) {}

    public:
        const SphereGravityModel::Iterator& getPointIterator() const;
        QGraphicsEllipseItem* getEllipseItem() const;
    };

private:
    SphereGravityModel* gravityModel;

    RotationTriple<double> perspectiveTriple;

    double projectionCircleRadius;

    QGraphicsEllipseItem* projectionCircle;

    QGraphicsLineItem* oxLineItem;
    QGraphicsLineItem* oyLineItem;
    QGraphicsLineItem* ozLineItem;

    QBrush backgroundBrush;
    std::list<PointContainer> pointContainerList;

public:
    SphereOrthogonalProjectionScene(SphereGravityModel*, QObject *parent = nullptr);

    void addMaterialPoint   (SphereGravityModel::Iterator pointIterator);
    void removeMaterialPoint(std::list<PointContainer>::iterator containerIterator);

    std::pair<double, double> projectOnXOY(Vector3d<double>);
    double                    projectOnOZ (Vector3d<double>);
    /// Rotates the perspectiveTriple around the OX axis
    void rotatePerspectiveOX(double angle);
    /// Rotates the perspectiveTriple around the OY axis
    void rotatePerspectiveOY(double angle);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    ~SphereOrthogonalProjectionScene();

    const RotationTriple<double>& getPerspectiveTriple() const;
    const Vector3d<double>& i() const;
    const Vector3d<double>& j() const;
    const Vector3d<double>& k() const;

    double getProjectionCircleRadius() const;
    void setProjectionCircleRadius(double newProjectionCircleRadius);

    SphereGravityModel *getGravityModel() const;

    QGraphicsEllipseItem *getProjectionCircle() const;

    const std::list<PointContainer>& getPointContainerList() const;

signals:
    void perspectiveChanged();
    void projectionCircleRadiusChanged();

public slots:
    void updatePointDisplay(PointContainer* pointContainer);
    void updateAllPointDisplays();
    void updateAxisLines();

};
