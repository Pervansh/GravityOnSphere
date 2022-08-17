#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtDebug>

void MainWindow::setTimeCoef(double newTimeCoef) {
    timeCoef = newTimeCoef;
}

void MainWindow::doStep() {
    gravityModel->step(1e-3 * modelStepTimer->interval() * timeCoef);
}

void MainWindow::onSpeedSliderValueChange() {
    int medium = (ui->speedSlider->minimum() + ui->speedSlider->maximum()) / 2;
    double e = (double)(ui->speedSlider->value() - medium) / 25;
    timeCoef = pow(2, e);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    gravityModel = new SphereGravityModel(SphereGravityModel::STANDART_C_FUNC);
    scene = new SphereOrthogonalProjectionScene(gravityModel, this);
    ui->sphereOrthogonalProjectionView->setScene(scene);

    modelStepTimer = new QTimer(this);
    modelStepTimer->setInterval(50);

    connect(modelStepTimer, &QTimer::timeout, this, &MainWindow::doStep);
    connect(modelStepTimer, &QTimer::timeout,
            scene, &SphereOrthogonalProjectionScene::updatePointDisplays);

    connect(ui->speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedSliderValueChange);

    /*
    scene->addMaterialPoint(gravityModel->addPoint({-0.5, 0.5, 0.5}, 1e-1));
    scene->addMaterialPoint(gravityModel->addPoint({0, 0.5, -0.7}, 1e-1));
    scene->addMaterialPoint(gravityModel->addPoint({0.7, 0, 0.7}, 1));
    */

    arcDrawer = new SphereOrthogonalProjectionArcDrawer(scene);

    connect(modelStepTimer, &QTimer::timeout,
            arcDrawer, &SphereOrthogonalProjectionArcDrawer::updateProjections);
    connect(scene, &SphereOrthogonalProjectionScene::perspectiveChange,
            arcDrawer, &SphereOrthogonalProjectionArcDrawer::updateProjections);

    arcDrawer->addEllipse({0, 1, 0}, {0, 0, 1}, QPen(Qt::yellow));
    arcDrawer->addArc({0, 1, 0}, {0, 0, 1}, QPen(Qt::red));

    onSpeedSliderValueChange();

    modelStepTimer->start();
}

MainWindow::~MainWindow() {
    delete ui;
    // delete scene;
    // delete modelStepTimer;
}

