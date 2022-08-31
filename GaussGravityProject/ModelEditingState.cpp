#include "ModelEditingState.h"

#include "ModelEditingGraphicsView.h"

ModelEditingState::ModelEditingState(ModelEditingGraphicsView* _view)
    : QObject(_view), view(_view) {}

void ModelEditingState::onViewMouseMoveEvent(QMouseEvent* event) {
    Q_UNUSED(event)
}

void ModelEditingState::onViewMousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ModelEditingState::onViewMouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ModelEditingState::onViewMouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ModelEditingState::onViewKeyPressEvent(QKeyEvent *event) {
    Q_UNUSED(event)
}

void ModelEditingState::onViewKeyReleaseEvent(QKeyEvent *event) {
    Q_UNUSED(event)
}

ModelEditingGraphicsView *ModelEditingState::getView() const {
    return view;
}

void ModelEditingState::setView(ModelEditingGraphicsView *newView) {
    view = newView;
}

void ModelEditingState::onDrop() {}

void ModelEditingState::raise() {
    view->changeState(this);
    onRaise();
    emit raised();
}

void ModelEditingState::enter(bool doRaise) {
    if (doRaise) {
        raise();
    } else {
        onDrop();
        emit dropped();
    }

    emit entering(doRaise);
}
