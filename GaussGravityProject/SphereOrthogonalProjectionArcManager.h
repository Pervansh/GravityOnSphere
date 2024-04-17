#pragma once
#include <QObject>
#include <list>
#include <map>
#include <utility>
#include "SphereOrthogonalProjectionScene.h"
#include <QGraphicsLineItem>
#include <QPen>
#include "Vector3d.h"

class SphereOrthogonalProjectionArcManager : public QObject {
    Q_OBJECT
public:
    class Arc {
        friend SphereOrthogonalProjectionArcManager;
    public:
        bool isClosed;

        const Vector3d<double> &getStartPlanePoint() const;
        const Vector3d<double> &getEndPlanePoint()   const;
        void setStartPlanePoint(const Vector3d<double> &newStartPlanePoint);
        void setEndPlanePoint  (const Vector3d<double> &newEndPlanePoint);

        const QPen &getForegroundPen() const;
        const QPen &getBackgroundPen() const;
        void setForegroundPen(const QPen &newForegroundPen);
        void setBackgroundPen(const QPen &newBackgroundPen);

    private:
        Vector3d<double> startPlanePoint;
        Vector3d<double> endPlanePoint;

        QPen foregroundPen;
        QPen backgroundPen;
        QGraphicsPathItem* foregroundItem;
        QGraphicsPathItem* backgroundItem;

        Arc(QGraphicsPathItem* _foregroundItem, QGraphicsPathItem* _backgroundItem)
            : foregroundItem(_foregroundItem), backgroundItem(_backgroundItem) {}
        ~Arc() {}
    };

private:
    SphereOrthogonalProjectionScene* scene;
    std::list<Arc*> allArcList;
    std::list<Arc*> vacantArcList;

    // For debug:
    QGraphicsLineItem* ellipseLine;

public:
    SphereOrthogonalProjectionArcManager(SphereOrthogonalProjectionScene* scene);

    Arc* addArc(const Vector3d<double>& startPoint,
                const Vector3d<double>& endPoint,
                const QPen& foregroundPen = QPen(),
                bool isClosed = false);
    Arc* addArc(const Vector3d<double>& startPoint,
                const Vector3d<double>& endPoint,
                const QPen& foregroundPen,
                const QPen& backgroundPen,
                bool isClosed = false);
    Arc* addClosedArc(const Vector3d<double>& planePoint1,
                      const Vector3d<double>& planePoint2,
                      const QPen& foregroundPen = QPen());
    Arc* addClosedArc(const Vector3d<double>& planePoint1,
                      const Vector3d<double>& planePoint2,
                      const QPen& foregroundPen,
                      const QPen& backgroundPen);
    void removeArc(Arc* arc);

    ~SphereOrthogonalProjectionArcManager();

public slots:
    void drawProjection(Arc* arc);
    void updateProjections();
};
