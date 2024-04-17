#pragma once

#include <QObject>
#include <QTimer>
#include <QToolButton>
#include <QAction>
#include "SphereGravityModel.h"

class ModelTimeManager : public QObject {
    Q_OBJECT
private:
    SphereGravityModel* model;

    QTimer* modelStepTimer;

    double timeCoef;

    QAction* resumeAction;
    QAction* pauseAction;

public:
    explicit ModelTimeManager(SphereGravityModel* _model,
                              QAction* _resumeAction,
                              QAction* _pauseAction,
                              QObject *parent = nullptr);

    QTimer* getModelStepTimer() const;
    double  getTimeCoef() const;
    QAction *getPauseAction() const;
    QAction *getResumeAction() const;

public slots:
    void resume();
    void pause();
    void doStep();

    void setTimeCoef(double newTimeCoef);

signals:

};
