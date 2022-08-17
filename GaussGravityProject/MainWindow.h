#pragma once

#include <QMainWindow>
#include <QTimer>
#include "SphereGravityModel.h"
#include "SphereOrthogonalProjectionScene.h"
#include "SphereOrthogonalProjectionArcDrawer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    SphereOrthogonalProjectionScene* scene;
    SphereGravityModel* gravityModel;

    SphereOrthogonalProjectionArcDrawer* arcDrawer;

    QTimer* modelStepTimer;

    double timeCoef;

public slots:
    void doStep();
    void onSpeedSliderValueChange();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setTimeCoef(double newTimeCoef);
};
