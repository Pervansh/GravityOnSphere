#include "NoEditingState.h"

void NoEditingState::onRaise() {
    getView()->setCursor(Qt::ArrowCursor);
    getView()->setMouseTracking(false);
}

void NoEditingState::onReplace() {

}

NoEditingState::NoEditingState(ModelEditingGraphicsView* view)
    : ModelEditingState(view) {}
