#pragma once

#include "ModelEditingState.h"
#include "ModelEditingGraphicsView.h"
#include "SphereGravityModel.h"
#include "SphereOrthogonalProjectionScene.h"
#include <QGraphicsEllipseItem>
#include <list>

class RemovePointEditingState : public ModelEditingState {
    Q_OBJECT

private:
    using containerIterator = std::list<SphereOrthogonalProjectionScene::PointContainer>::iterator;

    double removeAreaRadiusCoef = 0.1;
    QGraphicsEllipseItem* removeAreaItem;
    containerIterator lastSelectedPointIterator;

    double distFromCursorSq(QRectF);

protected:
    void onRaise()   override;
    void onReplace() override;

    void onViewMouseMoveEvent (QMouseEvent* event) override;
    void onViewMousePressEvent(QMouseEvent *event) override;

public:
    RemovePointEditingState(ModelEditingGraphicsView* view);
    void checkRemoveArea();

    double getRemoveAreaRadiusCoef() const;
    void setRemoveAreaRadiusCoef(double newRemoveAreaRadiusCoef);

public slots:
    void updateSelection();

signals:
    void pointWasRemoved(SphereGravityModel::Iterator pointIterator);
};
