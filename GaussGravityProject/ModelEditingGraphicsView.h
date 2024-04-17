#pragma once

#include <QGraphicsView>
#include "SphereOrthogonalProjectionScene.h"

class ModelEditingState;

class ModelEditingGraphicsView : public QGraphicsView {
    Q_OBJECT

private:
    SphereOrthogonalProjectionScene* modelScene;
    ModelEditingState* currentState;
    ModelEditingState* standartState;

    ModelEditingState* emptyState;

protected:
    void mouseMoveEvent       (QMouseEvent *event) override;
    void mousePressEvent      (QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent    (QMouseEvent *event) override;
    void keyPressEvent  (QKeyEvent *event)         override;
    void keyReleaseEvent(QKeyEvent *event)         override;

public:
    explicit ModelEditingGraphicsView(SphereOrthogonalProjectionScene* _modelScene, QWidget* parent = nullptr);
    explicit ModelEditingGraphicsView(QWidget* parent = nullptr);

    ModelEditingState* getCurrentState() const;

    SphereOrthogonalProjectionScene *getModelScene() const;
    void setModelScene(SphereOrthogonalProjectionScene *newModelScene);

    ModelEditingState* getStandartState() const;
    /// newStandartState can't be nullptr otherwise it will cause exception
    void setStandartState(ModelEditingState* newStandartState);

    ModelEditingState* getEmptyState() const;

public slots:
    void changeState(ModelEditingState* newState);
    void raiseStandartState();
    void enterStandartState(bool doRaise);

signals:
    void standartStateIsRaised();
    void standartStateIsEntering();
    void standartStateIsDropped();
    void stateWasChanged(ModelEditingState* previousState);
};
