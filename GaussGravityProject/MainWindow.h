#pragma once

#include <QMainWindow>
#include <QTimer>
#include "SphereGravityModel.h"
#include "SphereOrthogonalProjectionScene.h"
#include "SphereOrthogonalProjectionArcManager.h"

#include "ModelTimeManager.h"

#include <QToolButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    SphereOrthogonalProjectionScene* scene;
    SphereGravityModel* gravityModel;

    SphereOrthogonalProjectionArcManager* arcManager;

    ModelTimeManager* timeManager;

protected:
    virtual void resizeEvent(QResizeEvent *event);

public slots:
    void onSpeedSliderValueChange();
    void updateOrthogonalProjectionCircleRadius();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
