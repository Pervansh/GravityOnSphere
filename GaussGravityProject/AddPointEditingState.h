#pragma once

#include "ModelEditingState.h"
#include "ModelEditingGraphicsView.h"
#include "SphereGravityModel.h"

class AddPointEditingState : public ModelEditingState {
    Q_OBJECT

private:
    bool leftButtonWasPressed;

protected:
    void onRaise()   override;
    void onReplace() override;

    void onViewMouseMoveEvent   (QMouseEvent* event) override;
    void onViewMousePressEvent  (QMouseEvent *event) override;
    void onViewMouseReleaseEvent(QMouseEvent *event) override;

public:
    AddPointEditingState(ModelEditingGraphicsView* view);

signals:
    void pointWasAdded(SphereGravityModel::Iterator pointIterator);
};
