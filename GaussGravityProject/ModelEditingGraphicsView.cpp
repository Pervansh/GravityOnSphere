#include "ModelEditingGraphicsView.h"
#include "ModelEditingState.h"

#include <stdexcept>
#include <limits>

void ModelEditingGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    currentState->onViewMouseMoveEvent(event);
}

void ModelEditingGraphicsView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    currentState->onViewMousePressEvent(event);
}

void ModelEditingGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    QGraphicsView::mouseDoubleClickEvent(event);
    currentState->onViewMouseDoubleClickEvent(event);
}

void ModelEditingGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    currentState->onViewMouseReleaseEvent(event);
}

void ModelEditingGraphicsView::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    currentState->onViewKeyPressEvent(event);
}

void ModelEditingGraphicsView::keyReleaseEvent(QKeyEvent *event) {
    QGraphicsView::keyReleaseEvent(event);
    currentState->onViewKeyReleaseEvent(event);
}

ModelEditingGraphicsView::ModelEditingGraphicsView(SphereOrthogonalProjectionScene* _scene,
                                                   QWidget* parent)
    : QGraphicsView(_scene, parent),
      modelScene(_scene),
      standartState(nullptr),
      emptyState(new EmptyEditingState(this)) {
    setStandartState(new EmptyEditingState(this));
    currentState = emptyState;
}

ModelEditingGraphicsView::ModelEditingGraphicsView(QWidget *parent)
    : QGraphicsView(parent), standartState(nullptr), emptyState(new EmptyEditingState(this)) {
    setStandartState(new EmptyEditingState(this));
    currentState = emptyState;
}

void ModelEditingGraphicsView::setModelScene(SphereOrthogonalProjectionScene* newModelScene) {
    QGraphicsView::setScene(newModelScene);
    modelScene = newModelScene;
}

SphereOrthogonalProjectionScene *ModelEditingGraphicsView::getModelScene() const {
    return modelScene;
}

ModelEditingState *ModelEditingGraphicsView::getEmptyState() const {
    return emptyState;
}

void ModelEditingGraphicsView::raiseStandartState() {
    standartState->raise();
}

void ModelEditingGraphicsView::enterStandartState(bool doRaise) {
    standartState->enter(doRaise);
}

void ModelEditingGraphicsView::changeState(ModelEditingState* newState) {
    if (newState == nullptr)
        throw std::invalid_argument("newState in changeState() can't be nullptr! Use EmptyEditingState instead.");

    ModelEditingState* previousState = currentState;
    currentState = newState;

    previousState->onReplace();
    emit previousState->replaced();

    emit stateWasChanged(previousState);
}

ModelEditingState *ModelEditingGraphicsView::getStandartState() const {
    return standartState;
}

void ModelEditingGraphicsView::setStandartState(ModelEditingState* newStandartState) {
    if (newStandartState == nullptr)
        throw std::invalid_argument("newStandardState in getStandartState() can't be nullptr! Use EmptyEditingState instead.");

    if (standartState) {
        disconnect(standartState, &ModelEditingState::raised,
                   this, &ModelEditingGraphicsView::standartStateIsRaised);
        disconnect(standartState, &ModelEditingState::dropped,
                   this, &ModelEditingGraphicsView::standartStateIsDropped);
        disconnect(standartState, &ModelEditingState::entering,
                   this, &ModelEditingGraphicsView::standartStateIsEntering);
    }

    connect(newStandartState, &ModelEditingState::raised,
            this, &ModelEditingGraphicsView::standartStateIsRaised);
    connect(newStandartState, &ModelEditingState::dropped,
            this, &ModelEditingGraphicsView::standartStateIsDropped);
    connect(newStandartState, &ModelEditingState::entering,
            this, &ModelEditingGraphicsView::standartStateIsEntering);

    standartState = newStandartState;
}

ModelEditingState *ModelEditingGraphicsView::getCurrentState() const {
    return currentState;
}
