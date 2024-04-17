#include "RemovePointEditingState.h"


double RemovePointEditingState::getRemoveAreaRadiusCoef() const {
    return removeAreaRadiusCoef;
}

void RemovePointEditingState::setRemoveAreaRadiusCoef(double newRemoveAreaRadiusCoef) {
    removeAreaRadiusCoef = newRemoveAreaRadiusCoef;
}

double RemovePointEditingState::distFromCursorSq(QRectF rect) {
    auto mappedMouse = getView()->mapToScene(getView()->mapFromGlobal(QCursor::pos()));

    auto diffX = rect.x() + rect.width() / 2 - mappedMouse.x();
    auto diffY = rect.y() + rect.height() / 2 - mappedMouse.y();
    return diffX * diffX + diffY * diffY;
}

void RemovePointEditingState::onRaise() {
    getView()->setCursor(Qt::CursorShape::ArrowCursor);
}

void RemovePointEditingState::onReplace() {}

void RemovePointEditingState::onViewMouseMoveEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    checkRemoveArea();
    updateSelection();
}

void RemovePointEditingState::onViewMousePressEvent(QMouseEvent* event) {
    /*
    auto removeAreaRadius = removeAreaRadiusCoef
                            * getView()->getModelScene()->getProjectionCircleRadius();
    auto removeAreaRadiusSq = removeAreaRadius * removeAreaRadius;*/

    if ((event->buttons() & Qt::MouseButton::LeftButton)
       && lastSelectedPointIterator != getView()->getModelScene()->getPointContainerList().end()) {
        getView()->getModelScene()->removeMaterialPoint(lastSelectedPointIterator);
        emit pointWasRemoved(lastSelectedPointIterator->getPointIterator());
    }
}

RemovePointEditingState::RemovePointEditingState(ModelEditingGraphicsView* view)
    : ModelEditingState(view) {
    removeAreaItem = view->scene()->addEllipse(0, 0, 0, 0, QPen(Qt::yellow, Qt::PenStyle::DashLine));
    removeAreaItem->setVisible(false);
}

void RemovePointEditingState::checkRemoveArea() {
    auto removeAreaRadius = removeAreaRadiusCoef
                            * getView()->getModelScene()->getProjectionCircleRadius();
    auto removeAreaRadiusSq = removeAreaRadius * removeAreaRadius;

    auto containerList = getView()->getModelScene()->getPointContainerList();

    double minDistSq = 1e10;
    auto minIt = containerList.end();
    for (auto it = containerList.begin(); it != containerList.end(); ++it) {
        auto distSq = distFromCursorSq(it->getEllipseItem()->rect());

        if (distSq <= removeAreaRadiusSq && distSq <= minDistSq) {
            minDistSq = distSq;
            minIt = it;
        }
    }

    lastSelectedPointIterator = minIt;
}

void RemovePointEditingState::updateSelection() {
    if (lastSelectedPointIterator != getView()->getModelScene()->getPointContainerList().end()) {
        auto itemRect = lastSelectedPointIterator->getEllipseItem()->rect();
        removeAreaItem->setRect(itemRect.x() - 0.2 * itemRect.width(),
                                itemRect.y() - 0.2 * itemRect.height(),
                                1.2 * itemRect.width(), 1.2 * itemRect.height());
        removeAreaItem->setVisible(true);
    } else {
        removeAreaItem->setVisible(false);
    }
}
