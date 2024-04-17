#include "ModelTimeManager.h"

QTimer* ModelTimeManager::getModelStepTimer() const {
    return modelStepTimer;
}

void ModelTimeManager::resume() {
    modelStepTimer->start();
}

void ModelTimeManager::pause() {
    modelStepTimer->stop();
}

QAction *ModelTimeManager::getPauseAction() const {
    return pauseAction;
}

QAction *ModelTimeManager::getResumeAction() const {
    return resumeAction;
}

ModelTimeManager::ModelTimeManager(SphereGravityModel* _model,
                                   QAction* _resumeAction,
                                   QAction* _pauseAction,
                                   QObject* parent)
    : QObject(parent), model(_model), resumeAction(_resumeAction), pauseAction(_pauseAction) {

    modelStepTimer = new QTimer(this);
    modelStepTimer->setInterval(25);

    connect(modelStepTimer, &QTimer::timeout, this, &ModelTimeManager::doStep);

    connect(resumeAction, &QAction::triggered, this, &ModelTimeManager::resume);
    connect(pauseAction, &QAction::triggered, this, &ModelTimeManager::pause);
}

double ModelTimeManager::getTimeCoef() const {
    return timeCoef;
}

void ModelTimeManager::setTimeCoef(double newTimeCoef) {
    timeCoef = newTimeCoef;
}

void ModelTimeManager::doStep() {
    model->step(1e-3 * modelStepTimer->interval() * timeCoef);
}
