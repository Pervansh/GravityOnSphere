#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <algorithm>
#include <QResizeEvent>
#include <QtDebug>

#include "ModelEditingState.h"
#include "NoEditingState.h"
#include "AddPointEditingState.h"
#include "RemovePointEditingState.h"


void MainWindow::resizeEvent(QResizeEvent* event) {
    Q_UNUSED(event);
    updateOrthogonalProjectionCircleRadius();
}

void MainWindow::onSpeedSliderValueChange() {
    int medium = (ui->speedSlider->minimum() + ui->speedSlider->maximum()) / 2;
    double e = (double)(ui->speedSlider->value() - medium) / 25;
    timeManager->setTimeCoef(pow(2, e));
}

void MainWindow::updateOrthogonalProjectionCircleRadius() {
    auto size = ui->sphereOrthogonalProjectionView->size();
    scene->setProjectionCircleRadius(std::min(size.width(), size.height()) / 2.5);
    arcManager->updateProjections();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // GravityModel and Projection
    gravityModel = new SphereGravityModel(SphereGravityModel::STANDART_C_FUNC);
    scene = new SphereOrthogonalProjectionScene(gravityModel, this);

    // TimeManager
    timeManager = new ModelTimeManager(gravityModel,
                                       ui->resumeSimulationAction,
                                       ui->pauseSimulationAction,
                                       this);

    connect(timeManager->getModelStepTimer(), &QTimer::timeout,
            scene, &SphereOrthogonalProjectionScene::updateAllPointDisplays);

    connect(ui->speedSlider, &QSlider::valueChanged,
            this, &MainWindow::onSpeedSliderValueChange);

    // ModelEditingGraphicsView
    ui->sphereOrthogonalProjectionView->setModelScene(scene);

    //GraphicsView states
    ModelEditingState* noEditingState = new NoEditingState(ui->sphereOrthogonalProjectionView);
    connect(noEditingState, &ModelEditingState::raised,
            [=](){
        // IDK why it needs to be true
        ui->removeEditingAction->setChecked(true);
        ui->removeEditingAction->trigger();
    });

    ModelEditingState* addState = new AddPointEditingState(ui->sphereOrthogonalProjectionView);
    connect(ui->addPointEditingAction, &QAction::toggled,
            [=](bool checked) {
        if (checked) {
            addState->raise();
        } else {
            noEditingState->raise();
        }
    });

    RemovePointEditingState* removeState = new RemovePointEditingState(ui->sphereOrthogonalProjectionView);
    /*
    connect(ui->removeEditingAction, &QAction::toggled,
            [=](bool checked) {
        if (checked) {
            removeState->raise();
        } else {
            noEditingState->raise();
        }
    });
    connect(timeManager->getModelStepTimer(), &QTimer::timeout,
            removeState, &RemovePointEditingState::updateSelection);
    */

    /*
    connect(ui->addPointEditingAction, &QAction::toggled, addState, &ModelEditingState::enter);
    connect(addState, &ModelEditingState::dropped,
            ui->sphereOrthogonalProjectionView, &ModelEditingGraphicsView::raiseStandartState);
    */
    /*
    connect(ui->addPointModeAction, &QAction::toggled, addState, &ModelEditingState::enter);
    connect(addState, &ModelEditingState::entering, ui->sphereOrthogonalProjectionView, );
    */

    // PlayButton
    ui->playButton->setResumeAction(timeManager->getResumeAction());
    ui->playButton->setPauseAction (timeManager->getPauseAction());

    // ArcManager
    arcManager = new SphereOrthogonalProjectionArcManager(scene);

    connect(timeManager->getModelStepTimer(), &QTimer::timeout,
            arcManager, &SphereOrthogonalProjectionArcManager::updateProjections);
    connect(scene, &SphereOrthogonalProjectionScene::perspectiveChanged,
            arcManager, &SphereOrthogonalProjectionArcManager::updateProjections);

    ///*
    scene->addMaterialPoint(gravityModel->addPoint({-0.5, 0.5, 0.5}, 1e-2));
    scene->addMaterialPoint(gravityModel->addPoint({0, 0.5, -0.7}, 1e-2));
    scene->addMaterialPoint(gravityModel->addPoint({0.7, 0, 0.7}, 1e-1));
    //*/

    /*
    arcManager->addClosedArc({0, 1, 0}, {0, 0, 1}, QPen(Qt::yellow, 2, Qt::PenStyle::SolidLine));
    arcManager->addArc({0, 1, 0}, {0, 0, 1}, QPen(Qt::red, 3));
    */

    // Temporary
    updateOrthogonalProjectionCircleRadius();

    onSpeedSliderValueChange();

    timeManager->resume();

    timeManager->getResumeAction()->trigger();
}

MainWindow::~MainWindow() {
    delete gravityModel;
    delete ui;
    // delete scene;
    // delete modelStepTimer;
}

