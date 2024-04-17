#pragma once

#include "ModelEditingState.h"
#include "ModelEditingGraphicsView.h"

class NoEditingState : public ModelEditingState {
    Q_OBJECT

protected:
    void onRaise() override;
    void onReplace() override;

public:
    NoEditingState(ModelEditingGraphicsView* view);
};
