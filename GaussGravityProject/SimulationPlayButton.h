#pragma once

#include <QToolButton>
#include <QAction>

class SimulationPlayButton : public QToolButton {
    Q_OBJECT

private:
    QAction* resumeAction;
    QAction* pauseAction;

public:
    explicit SimulationPlayButton(QWidget* parent = nullptr);

    QAction *getResumeAction() const;
    void setResumeAction(QAction *newResumeAction);

    QAction *getPauseAction() const;
    void setPauseAction(QAction *newPauseAction);

public slots:
    void turnIntoResumeButton (bool actionChecked = true);
    void turnIntoPauseButton(bool actionChecked = true);
};
