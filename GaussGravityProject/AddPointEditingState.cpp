#include "AddPointEditingState.h"

#include <cmath>

void AddPointEditingState::onRaise() {
    getView()->setMouseTracking(true);
    leftButtonWasPressed = false;
}

void AddPointEditingState::onReplace() {

}

void AddPointEditingState::onViewMouseMoveEvent(QMouseEvent* event) {
    auto scene = getView()->getModelScene();

    auto circleRect =    scene->getProjectionCircle()->rect();
    auto circleRadius =  scene->getProjectionCircleRadius();

    auto circleCentreX = circleRect.x() + circleRect.width() / 2;
    auto circleCentreY = circleRect.y() + circleRect.height() / 2;

    auto mouseX = event->pos().x();
    auto mouseY = event->pos().y();
    auto mappedMouse = getView()->mapToScene(mouseX, mouseY);

    auto diffX = mappedMouse.x() - circleCentreX;
    auto diffY = mappedMouse.y() - circleCentreY;

    if (diffX * diffX + diffY * diffY <= circleRadius * circleRadius) {
        getView()->setCursor(Qt::CrossCursor);
    } else {
        getView()->setCursor(Qt::ForbiddenCursor);
    }
}

void AddPointEditingState::onViewMousePressEvent(QMouseEvent *event) {
    leftButtonWasPressed = event->buttons() & Qt::MouseButton::LeftButton;
}

void AddPointEditingState::onViewMouseReleaseEvent(QMouseEvent* event) {
    auto scene = getView()->getModelScene();

    auto circleRect =    scene->getProjectionCircle()->rect();
    auto circleRadius =  scene->getProjectionCircleRadius();

    auto circleCentreX = circleRect.x() + circleRect.width() / 2;
    auto circleCentreY = circleRect.y() + circleRect.height() / 2;

    auto mouseX = event->pos().x();
    auto mouseY = event->pos().y();
    auto mappedMouse = getView()->mapToScene(mouseX, mouseY);

    auto diffX = mappedMouse.x() - circleCentreX;
    auto diffY = mappedMouse.y() - circleCentreY;

    if (diffX * diffX + diffY * diffY <= circleRadius * circleRadius) {
        if (!(event->buttons() & Qt::MouseButton::LeftButton) && leftButtonWasPressed) {
            Vector3d<double> pointer = scene->i() * diffX
                                       + scene->j() * -diffY
                                       + scene->k() * std::sqrt(circleRadius * circleRadius
                                                                - diffX * diffX - diffY * diffY);
            auto it = getView()->getModelScene()->getGravityModel()->addPoint(pointer, 1e-1);
            scene->addMaterialPoint(it);

            emit pointWasAdded(it);
        }
    }
}

AddPointEditingState::AddPointEditingState(ModelEditingGraphicsView* view)
    : ModelEditingState(view) {}
