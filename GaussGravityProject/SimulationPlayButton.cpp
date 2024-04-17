#include "SimulationPlayButton.h"

QAction *SimulationPlayButton::getResumeAction() const {
    return resumeAction;
}

void SimulationPlayButton::setResumeAction(QAction* newResumeAction) {
    if (resumeAction) {
        disconnect(resumeAction, &QAction::triggered, this, &SimulationPlayButton::turnIntoPauseButton);
    }
    connect(newResumeAction, &QAction::triggered, this, &SimulationPlayButton::turnIntoPauseButton);

    resumeAction = newResumeAction;
}

QAction *SimulationPlayButton::getPauseAction() const {
    return pauseAction;
}

void SimulationPlayButton::setPauseAction(QAction* newPauseAction) {
    if (pauseAction) {
        disconnect(pauseAction, &QAction::triggered, this, &SimulationPlayButton::turnIntoResumeButton);
    }
    connect(newPauseAction, &QAction::triggered, this, &SimulationPlayButton::turnIntoResumeButton);

    pauseAction = newPauseAction;
}

void SimulationPlayButton::turnIntoResumeButton(bool actionChecked) {
    Q_UNUSED(actionChecked);
    setDefaultAction(resumeAction);
}

void SimulationPlayButton::turnIntoPauseButton(bool actionChecked) {
    Q_UNUSED(actionChecked);
    setDefaultAction(pauseAction);
}


SimulationPlayButton::SimulationPlayButton(QWidget *parent)
    : QToolButton(parent), resumeAction(nullptr), pauseAction(nullptr) {}
