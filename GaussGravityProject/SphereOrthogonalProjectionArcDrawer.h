#pragma once
#include <QObject>
#include <list>
#include <map>
#include <utility>
#include "SphereOrthogonalProjectionScene.h"
#include <QGraphicsLineItem>
#include "Vector3d.h"

class SphereOrthogonalProjectionArcDrawer : public QObject {
    Q_OBJECT
public:
    class Arc {
        friend SphereOrthogonalProjectionArcDrawer;
    public:
        QPen pen;
        Vector3d<double> startPlanePoint;
        Vector3d<double> endPlanePoint;
        bool isEllipse;

    private:
        QGraphicsEllipseItem* item;

        Arc(QGraphicsEllipseItem* _item) : item(_item) {};
        ~Arc() {}
    };

private:
    SphereOrthogonalProjectionScene* scene;
    std::list<Arc*> allArcList;
    std::list<Arc*> vacantArcList;

    // For debug:
    QGraphicsLineItem* ellipseLine;

public:
    SphereOrthogonalProjectionArcDrawer(SphereOrthogonalProjectionScene* scene);

    Arc* addArc(const Vector3d<double>& startPoint,
                const Vector3d<double>& endPoint,
                const QPen& pen = QPen(),
                bool isEllipse = false);
    Arc* addEllipse(const Vector3d<double>& planePoint1,
                    const Vector3d<double>& planePoint2,
                    const QPen& pen = QPen());
    void removeArc(Arc* arc);

    ~SphereOrthogonalProjectionArcDrawer();

public slots:
    void drawProjection(Arc* arc);
    void updateProjections();
};
